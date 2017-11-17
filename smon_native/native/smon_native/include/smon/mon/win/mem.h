#pragma once

namespace smon {
  namespace win {

    class Mem : public smon::Mem {
    public:
      Mem() {}
      virtual ~Mem() {}

      virtual void get(MemData& data) override
      {
        data.clear();

        MEMORYSTATUSEX info;
        info.dwLength = sizeof(MEMORYSTATUSEX);
        GlobalMemoryStatusEx(&info);

        DWORDLONG total = info.ullTotalPhys;
        DWORDLONG use = info.ullTotalPhys - info.ullAvailPhys;

        data.total_bytes_ = static_cast<uint64_t>(total);
        data.use_bytes_ = static_cast<uint64_t>(use);
      }

    };

  }
}

