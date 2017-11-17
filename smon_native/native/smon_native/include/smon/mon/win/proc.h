#pragma once

namespace smon {
  namespace win {

    class ProcCounter {
    public:
      ProcCounter() {}
      ~ProcCounter() {}

      PDH::Counter pid_;  // process id
      PDH::Counter cpu_;  // cpu usage
      PDH::Counter pmem_; // physical memory
      PDH::Counter vmem_; // virtual memory
      PDH::Counter io_total_; // total io bytes/sec
      PDH::Counter io_read_;  // read io bytes/sec
      PDH::Counter io_write_; // write io bytes/sec
      PDH::Counter th_cnt_;   // thread count
    };

    class Proc : public smon::Proc {
    private:
      PDH& pdh_;
      const std::string object_name_ = "Process";

      // key : process_name(lower case)
      std::map<std::string, std::vector<ProcCounter>> procs_;

      ProcList proc_list_;
      NetStat netstat_;

    public:
      Proc(PDH& pdh)
        : pdh_(pdh)
      {}

      virtual ~Proc() {}

      virtual void get(ProcData& data) override
      {
        data.clear();
        if (procs_.empty()) return;

        rearrange();

        netstat_.refresh();

        for (auto& it : procs_) {
          auto& process_name = it.first;
          auto& ctrs = it.second;


          auto& usages = data.proc_[process_name];

          for (auto& ctr : it.second) {
            ProcUsage u;
            u.pid_ = pdh_.get<uint32_t>(ctr.pid_);

            // pid 가 0이면 정상적인 수집이 이루어지기 전이므로 넘긴다.
            if (u.pid_ == 0) break;

            u.cpu_ = pdh_.get_ceil<uint32_t>(ctr.cpu_);
            u.pmem_ = pdh_.get<uint64_t>(ctr.pmem_);
            u.vmem_ = pdh_.get<uint64_t>(ctr.vmem_);
            u.io_total_ = pdh_.get<uint64_t>(ctr.io_total_);
            u.io_read_ = pdh_.get<uint64_t>(ctr.io_read_);
            u.io_write_ = pdh_.get<uint64_t>(ctr.io_write_);
            u.th_cnt_ = pdh_.get<uint32_t>(ctr.th_cnt_);

            auto cnt = netstat_.get(u.pid_);
            u.tcp_cnt_ = cnt.tcp_cnt_;
            u.tcp_listen_cnt_ = cnt.tcp_listen_cnt_;
            u.tcp_estab_cnt_ = cnt.tcp_estab_cnt_;
            u.udp_cnt_ = cnt.udp_cnt_;

            usages.push_back(u);
          }
          
        }
      }


      /**
      * @brief 현재 실행되고 있는 process 이름 목록
      * @detail
      * 중복되지 않고 정렬된 process 목록을 돌려줌
      * @return process list
      */
      virtual std::set<std::string> process_list()
      {
        //auto list = PDH::process_list();
        auto list = proc_list_.get();
        return std::set<std::string>(list.begin(), list.end());
      }


      /**
      * @brief 성능 모니터링할 프로세스 등록
      * @param string name 프로세스 이름(.exe 제외)
      */
      virtual void regist(const std::string& name)
      {
        const std::string& process_name = name; // Util::to_lower(name);

        auto it = procs_.find(process_name);
        if (it != procs_.end()) return;

        // 등록
        std::vector<ProcCounter> ctrs;

        //size_t cnt = counting(PDH::process_list_to_lower(), process_name);
        size_t cnt = counting(proc_list_.get(), process_name);

        for (size_t i = 0; i < cnt; ++i) {
          std::string path = PDH::make_instance_name(process_name, i);

          add(path, ctrs);
        }

        procs_.emplace(process_name, ctrs);
      }


      /**
      * @brief 성능 모니터링할 프로세스 해제
      * @param string name 프로세스 이름(.exe 제외)
      */
      virtual void unregist(const std::string& name)
      {
        const std::string& key = name;// Util::to_lower(name);

        auto it = procs_.find(key);
        if (it == procs_.end()) return;

        for (auto& ctr : it->second) {
          close(ctr);
        }
        procs_.erase(it);
      }

    private:

      /**
      * @brief 모니터링 중인 프로세스 갱신
      * @detail
      * 모니터링중인 프로세스의 인스턴스가 여러개인 경우
      * 추가 생성/종료가 반복될 수 있으므로 갱신을 수행
      */
      void rearrange()
      {
        //auto list = PDH::process_list_to_lower();
        auto list = proc_list_.get(); // get_to_lower();

        for (auto& it : procs_) {
          auto& process_name = it.first;
          auto& ctrs = it.second;

          
          int32_t ctrs_cnt = static_cast<int32_t>(ctrs.size());
          int32_t diff_cnt = counting(list, process_name) - ctrs_cnt;

          if (diff_cnt > 0) {
            // add
            for (size_t i = 0; i < diff_cnt; ++i) {
              std::string path = PDH::make_instance_name(process_name, i + ctrs_cnt);
              add(path, ctrs);
            }
          }
          else if (diff_cnt < 0) {
            // remove
            diff_cnt = ~diff_cnt + 1; // convert positive
            for (size_t i = 0; i < diff_cnt; ++i) {
              auto it = ctrs.rbegin();
              close(*it);
              ctrs.pop_back();
            }
          }
        }

      }


      /**
      * @brief list 에서 find 문자열 개수를 카운팅
      * @return find 문자열 개수
      */
      int32_t counting(const std::vector<std::string>& list, const std::string& find)
      {
        return static_cast<int32_t>(std::count(list.begin(), list.end(), find));
      }

      void add(const std::string& path, std::vector<ProcCounter>& ctrs)
      {
        ProcCounter ctr;
        ctr.pid_ = pdh_.open(Util::make_str("\\Process(%s)\\ID Process", path.c_str()));
        ctr.cpu_ = pdh_.open(Util::make_str("\\Process(%s)\\%% Processor Time", path.c_str()));
        ctr.pmem_ = pdh_.open(Util::make_str("\\Process(%s)\\Working Set", path.c_str()));
        ctr.vmem_ = pdh_.open(Util::make_str("\\Process(%s)\\Private Bytes", path.c_str()));
        ctr.io_total_ = pdh_.open(Util::make_str("\\Process(%s)\\IO Data Bytes/sec", path.c_str()));
        ctr.io_read_ = pdh_.open(Util::make_str("\\Process(%s)\\IO Read Bytes/sec", path.c_str()));
        ctr.io_write_ = pdh_.open(Util::make_str("\\Process(%s)\\IO Write Bytes/sec", path.c_str()));
        ctr.th_cnt_ = pdh_.open(Util::make_str("\\Process(%s)\\Thread Count", path.c_str()));
        ctrs.emplace_back(ctr);
      }

      void close(ProcCounter& ctr)
      {
        pdh_.close(ctr.pid_);
        pdh_.close(ctr.cpu_);
        pdh_.close(ctr.pmem_);
        pdh_.close(ctr.vmem_);
        pdh_.close(ctr.io_total_);
        pdh_.close(ctr.io_read_);
        pdh_.close(ctr.io_write_);
        pdh_.close(ctr.th_cnt_);
      }

    };

  }
}

