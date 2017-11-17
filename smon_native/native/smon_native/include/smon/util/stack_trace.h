﻿#pragma once

#include "char_array.h"

#if defined(WINDOWS)
# pragma warning(disable : 4091)
# include <DbgHelp.h>
# pragma warning(default : 4091)
# pragma comment(lib,"DbgHelp.Lib")
#else
#include <execinfo.h>
#include <dlfcn.h>
#include <cxxabi.h>
#endif

#if defined(WINDOWS)

// WINDOWS
namespace smon {

  class SymbolFinder
  {
  private:
    HANDLE handle_;

    struct SymbolInfo : public SYMBOL_INFO
    {
      char buffer_[256] = { 0, };

      SymbolInfo()
      {
        memset(this, 0x00, sizeof(*this));
        MaxNameLen = static_cast<ULONG>(sizeof(buffer_) - 1);
        SizeOfStruct = sizeof(SYMBOL_INFO);
      }
    };

    struct ImageHlpLine : public IMAGEHLP_LINE
    {
      ImageHlpLine()
      {
        memset(this, 0x00, sizeof(*this));
        SizeOfStruct = sizeof(IMAGEHLP_LINE);
      }
    };

  public:
    SymbolFinder()
    {
      handle_ = GetCurrentProcess();
      SymInitialize(handle_, nullptr, TRUE);
      SymSetOptions(SYMOPT_LOAD_LINES);
    }

    template <int size>
    void get(void* addr, CharArray<size>& carr)
    {
      if (!addr) return;

      char module_path[MAX_PATH] = { 0, };
      get_module_path(handle_, addr, module_path);

      SymbolInfo sinfo;
      ImageHlpLine line;
      get_symbol_line(handle_, addr, sinfo, line);

      carr.add("%p", addr);
      if (sinfo.NameLen != 0) {
        carr.add("  %s", sinfo.Name);
      }

      if (module_path) {
        carr.add("  %s", filename(module_path));
      }

      if (line.FileName) {
        carr.add("(%s:%d)", filename(line.FileName), line.LineNumber);
      }
      carr.add("\n");
    }

  private:
    template <size_t size>
    void get_module_path(HANDLE handle, void* addr, char(&buf)[size])
    {
      size_t module_base = SymGetModuleBase(
        handle,
        reinterpret_cast<size_t>(addr)
      );

      if (module_base == 0) {
        return;
      }

      GetModuleFileNameA(
        reinterpret_cast<HMODULE>(module_base),
        buf,
        size
      );
    }

    void get_symbol_line(HANDLE handle, void* addr, SymbolInfo& sinfo, ImageHlpLine& line)
    {
      if (SymFromAddr(
        handle,
        reinterpret_cast<size_t>(addr),
        0,
        &sinfo
      ) == FALSE) {
        return;
      }

      DWORD displacement = 0;
      SymGetLineFromAddr(
        handle,
        reinterpret_cast<size_t>(addr),
        &displacement,
        &line
      );
    }

    static const char* filename(const char* path)
    {
      if (!path) return path;

      const char* p = strrchr(path, '\\');
      if (!p) p = strrchr(path, '/');
      if (p) return p + 1;
      return path;
    }

  };


  class StackTrace
  {
  private:
    SymbolFinder symbol_finder_;

    struct StackFrame : public STACKFRAME
    {
      StackFrame()
      {
        memset(this, 0x00, sizeof(*this));
      }
    };

  public:
    StackTrace() {}

    ~StackTrace() {}

    std::string trace(int32_t depth = 64)
    {
      CONTEXT ctx;
      RtlCaptureContext(&ctx);
      return trace(&ctx, depth);
    }

    std::string trace(CONTEXT* ctx, int32_t depth = 64)
    {
      if (!ctx) {
        return "";
      }

      StackFrame stx;
      set_stack_frame(ctx, stx);

      CharArray<8192> carr;
      for (int i = 0; i < depth; ++i) {
        if (!next(ctx, stx)) {
          break;
        }

        symbol_finder_.get(
          reinterpret_cast<void*>(stx.AddrPC.Offset),
          carr
        );
      }
      return carr;
    }

    std::string trace_using_capture(int32_t depth = 64)
    {
      void** addrs = new void*[depth];

      ULONG hash = 0;
      int32_t cnt = CaptureStackBackTrace(0, depth, addrs, &hash);

      CharArray<8192> carr;
      for (int32_t i = 1; i < cnt && i < depth; ++i) {
        symbol_finder_.get(
          addrs[i], carr
        );
      }

      delete[] addrs;

      return carr;
    }

  private:
    void set_stack_frame(CONTEXT* ctx, StackFrame& stx)
    {
      memset(&stx, 0, sizeof(stx));

      stx.AddrPC.Mode = AddrModeFlat;
      stx.AddrStack.Mode = AddrModeFlat;
      stx.AddrFrame.Mode = AddrModeFlat;

#ifdef _WIN64
      stx.AddrPC.Offset = ctx->Rip;
      stx.AddrStack.Offset = ctx->Rsp;
      stx.AddrFrame.Offset = ctx->Rbp;
#else
      stx.AddrPC.Offset = ctx->Eip;
      stx.AddrStack.Offset = ctx->Esp;
      stx.AddrFrame.Offset = ctx->Ebp;
#endif
    }

    bool next(CONTEXT* ctx, StackFrame& stx)
    {
      return (StackWalk(
#ifdef _WIN64
        IMAGE_FILE_MACHINE_AMD64,
#elif _WIN32
        IMAGE_FILE_MACHINE_I386,
#else
        IMAGE_FILE_MACHINE_IA64,
#endif
        GetCurrentProcess(),
        GetCurrentThread(),
        &stx,
        ctx,
        NULL,
        SymFunctionTableAccess,
        SymGetModuleBase,
        NULL
      ) == TRUE);
    }

  };

}


#else

// LINUX
namespace smon {

  class StackTrace
  {
  public:
    StackTrace() {}

    ~StackTrace() {}

    std::string trace(int32_t depth = 64)
    {
      CharArray<8192> carr;

      void* callstack[depth];
      int size = backtrace(callstack, depth);
      char** symbols = backtrace_symbols(callstack, size);

      for (int i = 0; i < size && i < depth; ++i) {
        Dl_info info;
        if (dladdr(callstack[i], &info) && info.dli_sname) {
          char* demangled = nullptr;
          int status = -1;
          if (info.dli_sname[0] == '_') {
            demangled = abi::__cxa_demangle(info.dli_sname, nullptr, 0, &status);
          }
          const char* symbol = nullptr;
          if (status == 0) {
            symbol = demangled;
          }
          else {
            if (info.dli_sname == 0) {
              symbol = symbols[i];
            }
            else {
              symbol = info.dli_sname;
            }
          }

          carr.add(
            "0x%016X  %s + %zd\n",
            callstack[i], symbol,
            static_cast<char*>(callstack[i]) - static_cast<char*>(info.dli_saddr)
          );
        }
        else {
          carr.add(
            "0x%016X  %s\n",
            callstack[i],
            symbols[i]
          );
        }
      }

      return carr;
    }

  };

}
#endif

