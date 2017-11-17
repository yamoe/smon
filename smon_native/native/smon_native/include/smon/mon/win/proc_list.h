#pragma once

#include <psapi.h>
#include <TlHelp32.h>

namespace smon {
  namespace win {

    class ProcList {
    private:
      DWORD* pids_ = nullptr;
      DWORD pids_size_ = 0;

    public:
      ProcList(int size = 1024)
      {
        pids_ = new DWORD[size];
        pids_size_ = size * sizeof(DWORD);
      }

      ~ProcList()
      {
        if (pids_) {
          delete[] pids_;
          pids_ = nullptr;
          pids_size_ = 0;
        }
      }


      std::vector<std::string> get_to_lower()
      {
        return get(true);
      }

      std::vector<std::string> get(bool to_lower = false)
      {
        std::vector<std::string> list;

        memset(pids_, 0x00, pids_size_);

        DWORD ret_size = 0;
        if (!EnumProcesses(pids_, pids_size_, &ret_size)) {
          fprintf(stderr, "EnumProcesses : %d\n", GetLastError());
          return list;
        }

        std::string pname;
        size_t size = ret_size / sizeof(DWORD);
        for (size_t i = 0; i < size; ++i) {
          DWORD pid = pids_[i];
          if (pid != 0) {
            process_name(pid, pname, to_lower);
            if (!pname.empty()) {
              list.push_back(pname);
            }
          }
        }

        return list;
      }

      void process_name(DWORD pid, std::string& name, bool to_lower)
      {
        name.clear();

        HANDLE handle = OpenProcess(
          PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
          FALSE, pid);

        if (!handle) return;

        HMODULE mod;
        DWORD mod_size = 0;
        if (!EnumProcessModules(handle, &mod, sizeof(mod), &mod_size)) return;
        if (mod_size == 0) return;

        wchar_t str[MAX_PATH] = { 0, };
        get_module_basename(str, handle, mod);
        remove_exe(str);

        if (to_lower) {
          name = smon::Util::to_lower(smon::Util::w2u(str));
        }
        else {
          name = smon::Util::w2u(str);
        }

        CloseHandle(handle);
      }


      /*
      * page faults 가 많이 발생하므로 get() 사용 권장
      */
      __declspec(deprecated) std::vector<std::string> get_use_snapshot(bool to_lower = false)
      {
        std::vector<std::string> list;

        HANDLE handle = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe = { 0, };
        pe.dwSize = sizeof(PROCESSENTRY32);

        // page faults : Process32First, Process32Next
        if (Process32First(handle, &pe)) {
          do {
            wchar_t* str = pe.szExeFile;
            remove_exe(str);

            if (to_lower) {
              list.push_back(smon::Util::to_lower(smon::Util::w2u(str)));
            }
            else {
              list.push_back(smon::Util::w2u(str));
            }


          } while (Process32Next(handle, &pe));
        }
        CloseHandle(handle);
        return list;
      }

    private:

      template <std::size_t size>
      void get_module_filename(wchar_t(&str)[size], HANDLE handle, HMODULE mod)
      {
        // page faults
        GetModuleFileNameEx(handle, mod, str, size);
      }

      template <std::size_t size>
      void get_module_basename(wchar_t(&str)[size], HANDLE handle, HMODULE mod)
      {
        // page faults
        GetModuleBaseNameW(handle, mod, str, size);
      }

      void remove_exe(wchar_t* str)
      {
        // 확장자 제거. ".exe", ".EXE"
        size_t size = wcslen(str);
        if (size == 0) return;
        if (size > 4) {
          if (
            (str[size - 4] == L'.') &&
            ((str[size - 3] == L'e') || (str[size - 3] == L'E')) &&
            ((str[size - 2] == L'x') || (str[size - 2] == L'X')) &&
            ((str[size - 1] == L'e') || (str[size - 1] == L'E'))
            )
          {
            str[size - 4] = str[size - 3] = str[size - 2] = str[size - 1] = 0x00;
          }
        }
      }

    };

  }
}


