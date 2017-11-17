#pragma once

namespace smon {
  namespace win {

    class Disk : public smon::Disk {
    private:
      class IOCounter {
      public:
        PDH::Counter io_total_;
        PDH::Counter io_read_;
        PDH::Counter io_write_;
      };

      PDH& pdh_;
      std::map<std::string, IOCounter> counters_;

    public:
      Disk(PDH& pdh)
        : pdh_(pdh)
      {
        rearrange();
      }

      virtual ~Disk()
      {
        for (auto& it : counters_) {
          close_counter(it.second);
        }
      }

      virtual void get(DiskData& data) override
      {
        data.clear();

        rearrange();

        for (auto& it : counters_) {
          uint64_t total = 0, use = 0;
          WinUtil::disk_usage(it.first, total, use);

          auto& d = data.disk_[it.first];
          d.total_ = total;
          d.use_ = use;
          d.io_total_ = pdh_.get<uint64_t>(it.second.io_total_);
          d.io_read_ = pdh_.get<uint64_t>(it.second.io_read_);
          d.io_write_ = pdh_.get<uint64_t>(it.second.io_write_);
        }
      }

    private:
      void rearrange()
      {
        std::vector<std::string> a;
        for (auto& it : counters_) a.push_back(it.first);
        auto b = WinUtil::drives();

        // remove unplugged disk
        std::vector<std::string> remove;
        std::set_difference(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(remove));

        for (auto& drive : remove) {
          auto it = counters_.find(drive);
          close_counter(it->second);
          counters_.erase(it);
        }

        // add plugged disk
        std::vector<std::string> add;
        std::set_difference(b.begin(), b.end(), a.begin(), a.end(), std::back_inserter(add));

        for (auto& drive : add) {
          open_counter(drive);
        }
      }

      void open_counter(const std::string& drive)
      {
        IOCounter& io = counters_[drive];
        io.io_total_ = pdh_.open(Util::make_str("\\LogicalDisk(%s:)\\Disk Bytes/sec", drive.c_str()));
        io.io_read_ = pdh_.open(Util::make_str("\\LogicalDisk(%s:)\\Disk Read Bytes/sec", drive.c_str()));
        io.io_write_ = pdh_.open(Util::make_str("\\LogicalDisk(%s:)\\Disk Write Bytes/sec", drive.c_str()));
      }

      void close_counter(IOCounter& counter)
      {
        pdh_.close(counter.io_total_);
        pdh_.close(counter.io_read_);
        pdh_.close(counter.io_write_);
      }

    };

  }
}

