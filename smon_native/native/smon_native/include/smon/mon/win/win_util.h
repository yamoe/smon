#pragma once

class WinUtil {
public:

  static std::vector<std::string> drives()
  {
    std::vector<std::string> drives;
    DWORD len = ::GetLogicalDriveStringsA(0UL, nullptr);
    if (len == 0) return drives;

    std::string str(len, 0x00);
    ::GetLogicalDriveStringsA(len, &str[0]);

    for (int i = 0; i < str.size() / 4; ++i) {
      drives.push_back(std::string(1, str[i * 4]));    // C
    }
    return drives;
  }

  static void disk_usage(const std::string& drive, uint64_t& total, uint64_t& use)
  {
    std::string disk_path = drive + ":\\"; // C:\\

    ULARGE_INTEGER a = { 0, }, t = { 0, }, f = { 0, };
    GetDiskFreeSpaceExA(disk_path.c_str(), &a, &t, &f);

    total = static_cast<uint64_t>(t.QuadPart);
    use = static_cast<uint64_t>(t.QuadPart - f.QuadPart);
  }

};