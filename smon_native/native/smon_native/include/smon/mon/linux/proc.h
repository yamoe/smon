#pragma once

#include <unistd.h>
#include <sys/param.h>
#include <string>
#include <algorithm>

#include "../data.h"
#include "../interface.h"
#include "../../util/util.h"

#include "dir.h"
#include "file.h"

#include "uptime.h"
#include "proc_list.h"
#include "proc_stat.h"
#include "proc_io.h"
#include "proc_net_cnt.h"

namespace smon {
  namespace linux {

    class Proc : public smon::Proc {
		private:

			class Info {
			public:
				ProcStat::Stat stat_;
				ProcIO::IO io_;
			};


		private:
			typedef std::map<uint32_t, Info> Pids; // key : pid
			std::map<std::string, Pids> procs_; // key : process name

			double pre_uptime_ = 0;

    public:
      Proc() {}
      virtual ~Proc() override {}


			virtual void get(ProcData& data) override
			{
				data.clear();

				double uptime = Uptime::get();

				if (pre_uptime_ < uptime) {

					double diff_uptime = (pre_uptime_ == 0) ? 0 : uptime - pre_uptime_;


					ProcList::Map cur_procs = ProcList().get_map();

					for (auto& it : procs_) {
						const std::string& pname = it.first;
						Pids& pids = it.second;

						std::vector<ProcUsage>& usage_vec = data.proc_[pname];

						auto proc_it = cur_procs.find(pname);
						if (proc_it == cur_procs.end()) continue;

						for (uint32_t pid : proc_it->second) {

							Info info;
							info.stat_ = ProcStat::get(pid);
							info.io_ = ProcIO::get(pid);

							auto pid_it = pids.find(pid);
							if (pid_it != pids.end()) {

								Info& pre_info = pid_it->second;

								ProcUsage usage;
								usage.pid_ = pid;
								usage.cpu_ = info.stat_.cpu(diff_uptime, pre_info.stat_);
								usage.pmem_ = info.stat_.pmem_;
								usage.vmem_ = info.stat_.vmem_;

								if (!pre_info.io_.is_overflow(info.io_)) {
									usage.io_read_ = info.io_.read_ - pre_info.io_.read_;
									usage.io_write_ = info.io_.write_ - pre_info.io_.write_;
									usage.io_total_ = usage.io_read_ + usage.io_write_;
								}

								usage.th_cnt_ = info.stat_.th_cnt_;
								
								set_net_cnt(pid, usage);

								usage_vec.push_back(usage);
							}

							pids[pid] = info;
						}
					}
				}
				pre_uptime_ = uptime;
			}

    	virtual std::set<std::string> process_list() override
			{
				return ProcList().get_set();
			}
			
    	virtual void regist(const std::string& name) override
			{
				auto it = procs_.find(name);
				if (it != procs_.end()) return;

				procs_[name];
			}

    	virtual void unregist(const std::string& name) override
			{
				auto it = procs_.find(name);
				if (it == procs_.end()) return;

				procs_.erase(it);
			}
			
		private:
			void set_net_cnt(uint32_t pid, ProcUsage& usage)
			{
				auto inodes = ProcNetCnt::get_socket_inodes(pid);

				ProcNetCnt::NetCnt tcp = ProcNetCnt::get(ProcNetCnt::Type::Tcp, inodes, pid);
				ProcNetCnt::NetCnt tcp6 = ProcNetCnt::get(ProcNetCnt::Type::Tcp6, inodes, pid);
				ProcNetCnt::NetCnt udp = ProcNetCnt::get(ProcNetCnt::Type::Udp, inodes, pid);
				ProcNetCnt::NetCnt udp6 = ProcNetCnt::get(ProcNetCnt::Type::Udp6, inodes, pid);


				usage.tcp_cnt_ = tcp.total_ + tcp6.total_;
				usage.tcp_listen_cnt_ = tcp.listen_ + tcp6.listen_;
				usage.tcp_estab_cnt_ = tcp.estab_ + tcp6.estab_;
				usage.udp_cnt_ = udp.total_ + udp6.total_;
			}


    };

  }
}

