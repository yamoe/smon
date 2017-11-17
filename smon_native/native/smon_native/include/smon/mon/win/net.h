#pragma once

namespace smon {
  namespace win {

    class Net : public smon::Net {
    private:
      class Counter {
      public:
        PDH::Counter total_;
        PDH::Counter sent_;
        PDH::Counter recved_;
      };

      PDH pdh_;
      std::map<std::string, Counter> counters_;

    public:
      Net(PDH& pdh)
        : pdh_(pdh)
      {
        for (auto& interface_name : pdh.instance_list("Network Interface")) {
          auto& ctr = counters_[interface_name];
          ctr.total_ = pdh_.open(Util::make_str("\\Network Interface(%s)\\Bytes Total/sec", interface_name.c_str()));
          ctr.sent_ = pdh_.open(Util::make_str("\\Network Interface(%s)\\Bytes Sent/sec", interface_name.c_str()));
          ctr.recved_ = pdh_.open(Util::make_str("\\Network Interface(%s)\\Bytes Received/sec", interface_name.c_str()));
        }
      }

      virtual ~Net()
      {
        for (auto& it : counters_) {
          pdh_.close(it.second.total_);
          pdh_.close(it.second.sent_);
          pdh_.close(it.second.recved_);
        }
      }

      virtual void get(NetData& data) override
      {
        data.clear();
        for (auto& it : counters_) {
          auto& ctr = data.net_[it.first];
          ctr.total_ = pdh_.get<uint64_t>(it.second.total_);
          ctr.sent_ = pdh_.get<uint64_t>(it.second.sent_);
          ctr.recved_ = pdh_.get<uint64_t>(it.second.recved_);
        }
      }

    };

  }
}
