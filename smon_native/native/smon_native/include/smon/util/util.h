#pragma once

#if defined(WINDOWS)
#else
# include <cstdarg>
# include <cstring>
# include <unistd.h>
# include <sys/stat.h>
# include <iconv.h>
# include <termios.h>
# include <unistd.h>
#endif

namespace smon {

  class Util {
  public:

    /**
    * @brief UTC 현재 시간
    * @return UTC 현재 시간
    */
    static uint64_t utc_time()
    {
      time_t t = std::time(nullptr);
      return std::mktime(std::gmtime(&t));
    }

    /**
    * @brief UTC 문자열 시간
    * @return UTC 문자열 시간
    */
    static std::string utc_str(time_t t)
    {
      char buf[20] = { 0, };
      std::strftime(buf, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
      return buf;
    }

    /**
    * @brief cpu core 개수
    * @return cpu core 개수
    */
    static uint32_t core_count()
    {
      static uint32_t core_cnt = std::thread::hardware_concurrency();
      return core_cnt;
    }

    /**
     * @brief 읽기 편한 용량 단위로 변환
     * @detail
     *  b = 2048 인 경우 "2 KB" 리턴
     * @return 변환된 문자열
     */
    static std::string readable_bytes(uint64_t b)
    {
      static const char* units[] = { "B", "KB", "MB", "GB", "TB", "PB", "EB", "ZB", "YB" };

      int pos = 0;
      uint64_t q = b; //몫
      uint64_t r = 0; //나머지
      while (q >= 1024) {
        r = q % 1024;
        q /= 1024;
        ++pos;
      }

      //몫 + 나머지
      double ret = q + (static_cast<double>(r) / 1024);

      // 소수점 두번째 자리 반올림
      //ret = floor(10. * (ret + 0.05)) / 10.;
      ret = round(ret, -2);

      // 출력
      char buf[512] = { 0, };

#if defined(WINDOWS)
      sprintf_s(buf, "%g %s", ret, units[pos]);
#else
      sprintf(buf, "%g %s", ret, units[pos]);
#endif
      return buf;
    }

    /**
     * @brief 특정 자리수에서 반올림
     * @detail
     *  v = 0.05, pos = -2 인 경우 v = 0.01
     *  v = 150, pos = 2 인 경우 v = 200
     * @return 반올림된 수
     */
    static double round(double v, int pos)
    {
      if (pos == 0) return v;

      if (pos < 0) {
        pos = ~pos + 1;

        double t = 0;
        for (int i = 0; i < pos; ++i) {
          t = (t == 0) ? 1 : t * 10;
        }
        return ::round(t * v) / t;
      }

      if (pos > 0) {
        double t = 1;
        for (int i = 0; i < pos; ++i) t *= 10;
        return ::round(v / t) * t;
      }

      return v;
    }


    /**
     * @brief 문자열 조합
     * @detail
     *  prinf 형식의 문자열 조합.
     *  template을 통해 사용할 최대 버퍼 길이 지정 가능
     * @return 조합된 문자열
     */
    template <std::size_t size = 512>
    static std::string make_str(const char* format, ...)
    {
      char buf[size] = { 0, };

      va_list list;
      va_start(list, format);
#if defined(WINDOWS)
      vsnprintf_s(buf, size, size - 1, format, list);
#else
      vsnprintf(buf, size, format, list);
#endif
      va_end(list);

      return buf;
    }


    /**
    * @brief trim left
    * @detail trim character " \t\r\n\v"
    * @return left trimmed string
    */
    static std::string ltrim(const std::string& str, const std::string& chs = " \t\r\n\v")
    {
      //return str.erase(0, str.find_first_not_of(" \t\r\n\v"));
      return str.substr(str.find_first_not_of(chs));
    }

    /**
    * @brief trim right
    * @detail trim character " \t\r\n\v"
    * @return right trimmed string
    */
    static std::string rtrim(const std::string& str, const std::string& chs = " \t\r\n\v")
    {
      //return str.erase(str.find_last_not_of(" \t\r\n\v") + 1);
      return str.substr(0, str.find_last_not_of(chs) + 1);
    }

    /**
    * @brief trim both. left and right
    * @detail trim character " \t\r\n\v"
    * @return left right trimmed string
    */
    static std::string trim(const std::string& str, const std::string& chs = " \t\r\n\v")
    {
      return ltrim(rtrim(str, chs), chs);
    }

    static std::string to_lower(const std::string& str)
    {
      std::string ret(str.size(), 0x00);
      std::transform(str.begin(), str.end(), ret.begin(), ::tolower);
      return ret;
    }

    static std::string to_upper(const std::string& str)
    {
      std::string ret(str.size(), 0x00);
      std::transform(str.begin(), str.end(), ret.begin(), ::toupper);
      return ret;
    }

#if defined(WINDOWS)
#else

		static std::size_t iconv_to(const char* to, const char* from, char* in, std::size_t in_bytes_left, char* out, std::size_t out_bytes_left)
		{
			iconv_t cd = iconv_open(to, from);
			iconv(cd, &in, &in_bytes_left, &out, &out_bytes_left);
			iconv_close(cd);
			return out_bytes_left;
		}
#endif

    // wide -> utf8
    static std::string w2u(const std::wstring& in)
    {
#if defined(WINDOWS)
      int len = ::WideCharToMultiByte(CP_UTF8, 0, in.c_str(), -1, 0, 0, 0, 0);
      std::string out(len - 1, 0);
      ::WideCharToMultiByte(CP_UTF8, 0, in.c_str(), -1, const_cast<char*>(out.c_str()), len, 0, 0);
      return out;
#else
      std::string out(in.size() * 2, 0x00);
      std::size_t bytes_left = iconv_to(
        "UTF-8", "WCHAR_T",
        reinterpret_cast<char*>(const_cast<wchar_t*>(in.c_str())),
        (in.size() * sizeof(wchar_t)),
        const_cast<char*>(out.c_str()),
        out.size()
      );
      out.resize(out.size() - bytes_left);
      return out;
#endif
    }

  };

}


