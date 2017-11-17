﻿#pragma once

namespace smon {

  template <int size = 8192>
  class CharArray
  {
  private:
    char arr_[size] = { 0 };
    int pos_ = 0;

  public:
    CharArray() {}

    operator const void*() { return arr_; }

    operator const char*() { return arr_; }

    operator std::string() { return arr_; }

    int len() { return pos_; }

    CharArray& reset()
    {
      pos_ = 0;
      return *this;
    }

    CharArray& add(const char* format, ...)
    {
      if (pos_ == -1) {
        return *this;
      }

      va_list list;
      va_start(list, format);
#if defined(WINDOWS)
      int ret = vsnprintf_s(arr_ + pos_, size - pos_, size - pos_ - 1, format, list);
#else
      int ret = vsnprintf(arr_ + pos_, size - pos_, format, list);
#endif
      va_end(list);

      if (ret == -1) {
        pos_ = -1;
      }
      else {
        pos_ += ret;
      }
      return *this;
    }

    char* ch()
    {
      return arr_;
    }

  };

}
