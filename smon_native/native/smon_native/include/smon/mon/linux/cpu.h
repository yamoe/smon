#pragma once

#include "file.h"

namespace smon {
  namespace linux {

    class CPU : public smon::CPU {
		private:
			class Data {
			public:
				uint64_t user_ = 0;
				uint64_t user_low_ = 0;
				uint64_t sys_ = 0;
				uint64_t idle_ = 0;

				uint32_t percent(Data& now)
				{
					if (now.user_ < user_ ||
							now.user_low_ < user_low_ ||
							now.sys_ < sys_ ||
							now.idle_ < idle_)
					{
						return 0;
					}
					uint64_t sum = 
						(now.user_ - user_) +
						(now.user_low_ - user_low_) +
						(now.sys_ - sys_);
					double percent = sum;
					sum += now.idle_ - idle_;
					percent /= sum;
					percent *= 100;
					return static_cast<uint32_t>(Util::round(percent, -1));
				}
				
			};

		private:
			const char* filename_ = "/proc/stat";
			Data pre_total_;
			std::vector<Data> pre_cores_;

    public:
      CPU() {}
      virtual ~CPU() override
			{
			}

			virtual void get(CpuData& data) override
			{
				data.clear();

				File file;
				if (!file.open(filename_)) {
					Log::err("%s %s", filename_, file.err().c_str());
					return;
				}


				Data now;
				char line[256] = {0,};
				char cpu[10] = {0,};

				while (file.get(line)) {
					if (strncmp(line, "cpu", 3)) {
						break;
					}

					sscanf(line, "%s %lu %lu %lu %lu", cpu, &now.user_, &now.user_low_, &now.sys_, &now.idle_);

					size_t len = strlen(cpu);
					if (len == 3) {
						// total
						data.total_ = pre_total_.percent(now);
						pre_total_ = now;
					} else {
						// core
						int idx = atoi(cpu + 3);
						if (pre_cores_.size() < static_cast<size_t>(idx + 1)) {
							pre_cores_.push_back(now);
							data.cores_.push_back(0);
						} else {
							Data& pre = pre_cores_[idx];
							data.cores_.push_back(pre.percent(now));
							pre = now;
						}
					}
				}
			}
    };

  }
}

