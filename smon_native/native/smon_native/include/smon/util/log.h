#pragma once

namespace smon {

  class Log {
  public:
    template <std::size_t size = 1024>
    static void err(const char* format, ...)
    {
      va_list args;
      va_start(args, format);
      err(format, args);
      va_end(args);
    }

    template <std::size_t size = 1024>
    static void err(const char* format, va_list args)
    {
      char buf[size] = { 0, };
#if defined(WINDOWS)
      vsnprintf_s(buf, size, size - 1, format, args);
#else
      vsnprintf(buf, size, format, args);
#endif
      fprintf(stderr, "%s\n%s\n", buf, StackTrace().trace().c_str());
    }

    template <std::size_t size = 1024>
    static void debug(const char* format, ...)
    {
#if defined(DEBUG)
      char buf[size] = { 0, };

      va_list args;
      va_start(args, format);
#if defined(WINDOWS)
      vsnprintf_s(buf, size, size - 1, format, args);
#else
      vsnprintf(buf, size, format, args);
#endif
      va_end(args);
      printf("%s\n", buf);
#endif
    }
  };

}

