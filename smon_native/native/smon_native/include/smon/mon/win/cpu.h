#pragma once

namespace smon {
  namespace win {

    class CPU : public smon::CPU {
    private:
      PDH& pdh_;
      PDH::Counter total_;
      std::vector<PDH::Counter> cores_;

    public:
      CPU(PDH& pdh)
        : pdh_(pdh)
      {
        total_ = pdh_.open("\\Processor(_Total)\\% Processor Time");

        uint32_t cnt = Util::core_count();
        for (uint32_t i = 0; i < cnt; ++i) {
          std::string path = Util::make_str("\\Processor(%d)\\%% Processor Time", i);
          cores_.emplace_back(pdh_.open(path));
        }
      }

      virtual ~CPU()
      {
        pdh_.close(total_);
        for (auto& counter : cores_) {
          pdh_.close(counter);
        }
      }

      virtual void get(CpuData& data) override
      {
        data.clear();
        data.total_ = pdh_.get_ceil<uint32_t>(total_);

        for (auto& counter : cores_) {
          data.cores_.push_back(pdh_.get_ceil<uint32_t>(counter));
        }
      }

    };

  }
}

