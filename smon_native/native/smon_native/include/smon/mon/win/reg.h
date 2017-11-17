#pragma once

namespace smon {

  class REG {
  private:

    HKEY hkey_ = 0;
    const std::string& sub_key_;
    const std::string& value_name_;

    HKEY key_ = 0;
    DWORD type_ = 0;
    DWORD size_ = 0;

  public:

    REG(const HKEY hkey, const std::string& sub_key, const std::string& value_name)
      : hkey_(hkey)
      , sub_key_(sub_key)
      , value_name_(value_name)

    {
      open();
      set_size_and_type();
    }

    ~REG()
    {
      if (key_) RegCloseKey(key_);
    }

    uint32_t dword()
    {
      if (size_ != 4) return 0;
      DWORD v = 0;
      RegQueryValueExA(key_, value_name_.c_str(), 0, NULL, (BYTE*)&v, &size_);
      return v;
    }

    uint64_t qword()
    {
      if (size_ != 8) return 0;
      uint64_t v = 0;
      RegQueryValueExA(key_, value_name_.c_str(), 0, NULL, (BYTE*)&v, &size_);
      return v;
    }

    std::string string()
    {
      if (size_ < 2) return "";
      std::string v(size_ - 1, 0x00);
      RegQueryValueExA(key_, value_name_.c_str(), 0, NULL, (BYTE*)&v[0], &size_);
      return v;
    }

    std::vector<std::string> multi_string()
    {
      std::string buf(size_, 0x00);

      BYTE* pos = reinterpret_cast<BYTE*>(&buf[0]);
      RegQueryValueExA(key_, value_name_.c_str(), 0, NULL, pos, &size_);

      std::vector<std::string> v;
      for (DWORD i = 0; i < size_; ) {
        if (pos[i] == 0x00) break;
        v.push_back(reinterpret_cast<const char*>(pos + i));

        i += static_cast<DWORD>(strlen(reinterpret_cast<const char*>(pos + i))) + 1;
      }
      return v;
    }

    std::wstring link()
    {
      if (size_ < 4) return L"";
      std::wstring v(size_ / sizeof(wchar_t), 0x00);
      RegQueryValueExA(key_, value_name_.c_str(), 0, NULL, (BYTE*)&v[0], &size_);
      return v;
    }

    std::vector<BYTE> binary()
    {
      std::vector<BYTE> v(size_, 0x00);
      RegQueryValueExA(key_, value_name_.c_str(), 0, NULL, (BYTE*)&v[0], &size_);
      return v;
    }

  private:
    void open() {
      LONG ret = RegOpenKeyExA(hkey_, sub_key_.c_str(), 0, KEY_QUERY_VALUE, &key_);
      if (ret != ERROR_SUCCESS) {
        Log::err("RegOpenKeyExA[%ld] %s\\%s", ret, hkey_str(), sub_key_.c_str());
      }
    }

    void set_size_and_type()
    {
      if (!key_) return;

      LONG ret = RegQueryValueExA(key_, value_name_.c_str(), 0, &type_, NULL, &size_);
      if (ret != ERROR_SUCCESS) {
        Log::err("RegQueryValueExA[%ld] %s\\%s\\%s", ret, hkey_str(), sub_key_.c_str(), value_name_.c_str());
        return;
      }
    }

    const char* hkey_str()
    {
      if (hkey_ == HKEY_CLASSES_ROOT) return "HKEY_CLASSES_ROOT";
      if (hkey_ == HKEY_CURRENT_CONFIG) return "HKEY_CURRENT_CONFIG";
      if (hkey_ == HKEY_CURRENT_USER) return "HKEY_CURRENT_USER";
      if (hkey_ == HKEY_LOCAL_MACHINE) return "HKEY_LOCAL_MACHINE";
      if (hkey_ == HKEY_USERS) return "HKEY_USERS";
      return "";
    }

  };
}

