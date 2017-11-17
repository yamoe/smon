#pragma once

#include <sys/sysinfo.h>

namespace smon {
  namespace linux {

    class Mem : public smon::Mem {
    public:
      Mem() {}
      virtual ~Mem() override {}

			virtual void get(MemData& data) override
			{
				data.clear();

				// can use /proc/meminfo alternatively
				struct sysinfo info;
				if (sysinfo(&info) == -1) {
					return;
				}

				/*
				// total virtual memory
				data.total_bytes_ = (info.totalram + info.totalswap) * info.mem_unit;
				// use virtual memory
				data.use_bytes_ = ((info.totalram - info.freeram) + (info.totalswap - info.freeswap)) * info.mem_unit;
				*/

				// total physical memory
				data.total_bytes_ = (info.totalram) * info.mem_unit;

				// use physical memory
				data.use_bytes_ = (info.totalram - info.freeram - info.bufferram) * info.mem_unit;
			}

    };

  }
}

