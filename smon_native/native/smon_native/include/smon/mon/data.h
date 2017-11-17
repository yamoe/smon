
#pragma once

namespace smon {

  class CpuData {
  public:
    uint64_t utc_ = Util::utc_time();
    uint32_t total_ = 0;
    std::vector<uint32_t> cores_;

    void clear()
    {
      utc_ = Util::utc_time();
      total_ = 0;
      cores_.clear();
    }

    std::string to_str()
    {
      std::string str;
      for (size_t i = 0; i < cores_.size(); ++i) {
        str += Util::make_str("%3u", cores_[i]);
        if ((i + 1) != cores_.size()) str += " ";
      }

      return Util::make_str(
        "(%s) %3u - %s",
        Util::utc_str(utc_).c_str(),
        total_,
        str.c_str()
      );
    }
  };


  class MemData {
  public:
    uint64_t utc_ = Util::utc_time();
    uint64_t total_bytes_ = 0;
    uint64_t use_bytes_ = 0;

    void clear()
    {
      utc_ = Util::utc_time();
      total_bytes_ = 0;
      use_bytes_ = 0;
    }

    std::string to_str()
    {
      return Util::make_str(
        "(%s) %s / %s",
        Util::utc_str(utc_).c_str(),
        Util::readable_bytes(use_bytes_).c_str(),
        Util::readable_bytes(total_bytes_).c_str()
      );
    }
  };


  class DiskUsage {
  public:
    uint64_t total_ = 0;
    uint64_t use_ = 0;
    uint64_t io_total_ = 0;
    uint64_t io_read_ = 0;
    uint64_t io_write_ = 0;
  };

  class DiskData {
  public:
    uint64_t utc_ = Util::utc_time();
    std::map<std::string, DiskUsage> disk_;

    void clear()
    {
      utc_ = Util::utc_time();
      disk_.clear();
    }

    std::string to_str()
    {
      std::string str = Util::utc_str(utc_) + "\n";

      uint32_t cnt = 1;
      for (auto it : disk_) {
        const std::string& drv = it.first;
        DiskUsage& d = it.second;

        str += Util::make_str("%s - total: %s, use: %s, io_total: %s, io_read: %s, io_write: %s",
          drv.c_str(),
          Util::readable_bytes(d.total_).c_str(),
          Util::readable_bytes(d.use_).c_str(),
          Util::readable_bytes(d.io_total_).c_str(),
          Util::readable_bytes(d.io_read_).c_str(),
          Util::readable_bytes(d.io_write_).c_str()
        );
        if (cnt != disk_.size()) str += "\n";
        ++cnt;
      }
      return str;
    }
  };


  class NetUsage {
  public:
    uint64_t total_ = 0;
    uint64_t sent_ = 0;
    uint64_t recved_ = 0;
  };

  class NetData {
  public:
    uint64_t utc_ = Util::utc_time();
    std::map<std::string, NetUsage> net_;

    void clear()
    {
      utc_ = Util::utc_time();
      net_.clear();
    }

    std::string to_str()
    {
      std::string str = Util::utc_str(utc_) + "\n";

      uint32_t cnt = 1;
      for (auto it : net_) {
        auto& d = it.second;

        str += Util::make_str("%s - total: %s, sent:%s, recved: %s",
          it.first.c_str(),
          Util::readable_bytes(d.total_).c_str(),
          Util::readable_bytes(d.sent_).c_str(),
          Util::readable_bytes(d.recved_).c_str()
        );
        if (cnt != net_.size()) str += "\n";
        ++cnt;
      }
      return str;
    }
  };



  class ProcUsage {
  public:
    uint32_t pid_ = 0;
    uint32_t cpu_ = 0;
    uint64_t pmem_ = 0;
    uint64_t vmem_ = 0;
    uint64_t io_total_ = 0;
    uint64_t io_read_ = 0;
    uint64_t io_write_ = 0;
    uint32_t th_cnt_ = 0; // thread count
    uint32_t tcp_cnt_ = 0;
    uint32_t tcp_listen_cnt_ = 0;
    uint32_t tcp_estab_cnt_ = 0;
    uint32_t udp_cnt_ = 0;
  };

  class ProcData {
  public:
    uint64_t utc_ = Util::utc_time();
    // key: process name
    std::map<std::string, std::vector<ProcUsage>> proc_;

    void clear()
    {
      utc_ = Util::utc_time();
      proc_.clear();
    }

    std::string to_str()
    {
      std::string str = Util::utc_str(utc_) + "\n";

      for (auto& it : proc_) {
        auto& pname = it.first;
        for (auto& jt : it.second) {
          auto& u = jt;
          str += Util::make_str(
            "%s[%u], cpu:%2u%%, pmem: %s, vmem: %s, io_total: %s, io_read: %s, io_write: %s, th_cnt: %u, "
            "tcp_cnt: %u, tcl_listne_cnt: %u, tcp_estab_cnt: %u, udp_cnt: %u\n",
            pname.c_str(),
            u.pid_,
            u.cpu_,
            Util::readable_bytes(u.pmem_).c_str(),
            Util::readable_bytes(u.vmem_).c_str(),
            Util::readable_bytes(u.io_total_).c_str(),
            Util::readable_bytes(u.io_read_).c_str(),
            Util::readable_bytes(u.io_write_).c_str(),
            u.th_cnt_,
            u.tcp_cnt_,
            u.tcp_listen_cnt_,
            u.tcp_estab_cnt_,
            u.udp_cnt_
          );
        }
      }
      return str;
    }
  };



  class SysData
  {
  public:
    uint64_t utc_ = Util::utc_time();
    std::string host_name_;
    std::string os_name_;
    std::string processor_;
    std::string baseboard_;
    std::string ram_;
    std::string disk_;
    std::string ip_;

    std::string to_str()
    {
      return Util::make_str(
        "(%s) hostname: %s, os: %s, cpu: %s, baseboard: %s, ram: %s, disk: %s, ip: %s",
        Util::utc_str(utc_).c_str(),
        host_name_.c_str(),
        os_name_.c_str(),
        processor_.c_str(),
        baseboard_.c_str(),
        ram_.c_str(),
        disk_.c_str(),
        ip_.c_str()
      );
    }

  };


}

