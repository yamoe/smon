#pragma once

#include "win_util.h"
#include "reg.h"
#include "net_stat.h"
#include "proc_list.h"
#include "pdh.h"
#include "cpu.h"
#include "disk.h"
#include "mem.h"
#include "net.h"
#include "proc.h"
#include "sys.h"
#include "timer.h"

namespace smon {
  namespace win {

    class Manager : public smon::Manager, public smon::TimerReceiver {
    private:
      std::atomic<uint32_t> period_sec_ = 1;
      std::mutex mutex_;
      std::set<Callback*> cbs_;

      Timer timer_;

      PDH pdh_;

      SYS sys_;

      CPU cpu_ = CPU(pdh_);
      CpuData cpu_data_;

      Mem mem_;
      MemData mem_data_;

      Disk disk_ = Disk(pdh_);
      DiskData disk_data_;

      Net net_ = Net(pdh_);
      NetData net_data_;

      Proc proc_ = Proc(pdh_);
      ProcData proc_data_;


    public:
      Manager() {}

      virtual ~Manager()
      {
        stop();
      }

      virtual void set_callback(Callback* cb) override
      {
        cbs_.insert(cb);
      }

      virtual void start() override
      {
        timer_.start(this);
      }

      virtual void stop() override
      {
        timer_.stop();
      }

      virtual void set_period(uint32_t sec) override
      {
        period_sec_ = sec;
      }


      virtual CpuData cpu() override
      {
        std::lock_guard<std::mutex> lock(mutex_);
        return cpu_data_;
      }

      virtual MemData mem() override
      {
        std::lock_guard<std::mutex> lock(mutex_);
        return mem_data_;
      }

      virtual SysData sys() override
      {
        return sys_.get();
      }

      virtual DiskData disk() override
      {
        std::lock_guard<std::mutex> lock(mutex_);
        return disk_data_;
      }

      virtual NetData net() override
      {
        std::lock_guard<std::mutex> lock(mutex_);
        return net_data_;
      }

      virtual std::set<std::string> proc_list() override
      {
        return proc_.process_list();
      }

      virtual void regist_proc(const std::string& pname) override
      {
        std::lock_guard<std::mutex> lock(mutex_);
        proc_.regist(pname);
      }

      virtual void unregist_proc(const std::string& pname) override
      {
        std::lock_guard<std::mutex> lock(mutex_);
        proc_.unregist(pname);
      }

      virtual ProcData proc() override
      {
        std::lock_guard<std::mutex> lock(mutex_);
        return proc_data_;
      }


    private:
      virtual void on_timer(uint32_t seq) override
      {
        if (seq % period_sec_ != 0) return;
        pdh_.update();
        collect();
        for (Callback* cb : cbs_) {
          cb->on_callback();
        }
      }

      void collect()
      {
        std::lock_guard<std::mutex> lock(mutex_);
        cpu_.get(cpu_data_);
        mem_.get(mem_data_);
        disk_.get(disk_data_);
        net_.get(net_data_);
        proc_.get(proc_data_);
      }


    };
    

  }
}
