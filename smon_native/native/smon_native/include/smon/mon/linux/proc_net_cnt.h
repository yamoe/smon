#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "file.h"
#include "dir.h"
#include "../../util/util.h"
#include "../../util/log.h"

namespace smon {
	namespace linux {

		class ProcNetCnt {
		private:
			enum class TCP_STATES {
				TCP_ESTABLISHED = 1,
				TCP_SYN_SENT,
				TCP_SYN_RECV,
				TCP_FIN_WAIT1,
				TCP_FIN_WAIT2,
				TCP_TIME_WAIT,
				TCP_CLOSE,
				TCP_CLOSE_WAIT,
				TCP_LAST_ACK,
				TCP_LISTEN,
				TCP_CLOSING
			};

		public:
			enum class Type {
				Tcp,
				Tcp6,
				Udp,
				Udp6
			};

			class NetCnt {
			public:
				uint32_t total_ = 0;
				uint32_t listen_ = 0;
				uint32_t estab_ = 0;
			};



		public:
			ProcNetCnt() {}
			~ProcNetCnt() {}


			static NetCnt get(Type type, std::vector<uint64_t>& inodes, uint32_t pid)
			{
				if (inodes.empty()) return NetCnt();

				switch (type) {
					case Type::Tcp:
						return get(Util::make_str("/proc/%u/net/tcp", pid), inodes);
					case Type::Tcp6:
						return get(Util::make_str("/proc/%u/net/tcp6", pid), inodes);
					case Type::Udp:
						return get(Util::make_str("/proc/%u/net/udp", pid), inodes);
					case Type::Udp6:
						return get(Util::make_str("/proc/%u/net/udp6", pid), inodes);
					default:
						return NetCnt();
				}
			}

			static NetCnt get(const std::string& path, std::vector<uint64_t>& inodes)
			{
				NetCnt net_cnt;

				File file;
				if (!file.open(path.c_str())) {
					Log::err("%s %s", path.c_str(), file.err().c_str());
					return net_cnt;
				}

				
				char line[512] = {0,};
				char dum[128] = {0,};
				char st[4] = {0,};
				uint64_t inode = 0;

				bool is_first_line = true;
				while (file.get(line)) {
					if (is_first_line) {
						is_first_line = false;
						continue;
					}

					sscanf(line,
						"%s %s %s %s %s %s %s %s %s %lu %s",
					//  1  2  3  4  5  6  7  8  9 10 11
						dum, // 1. sl
						dum, // 2. local_address
						dum, // 3. remote_address
						st,  // 4. st
						dum, // 5. tx_queue & rx_queue
						dum, // 6. tr & tm->when
						dum, // 7. retrnsmt
						dum, // 8. uid
						dum, // 9. timeout
						&inode, // 10. inode
						dum // 11. else ([tcp, tcp6] ref, pointer, drops. [upd, udp6] : none)
					);

					if (std::find(inodes.begin(), inodes.end(), inode) == inodes.end()) {
						continue;
					}


					TCP_STATES stat = static_cast<TCP_STATES>(strtoul(st, 0, 16));
					switch (stat) {
						case TCP_STATES::TCP_ESTABLISHED:
							++net_cnt.estab_;
							break;
						case TCP_STATES::TCP_LISTEN:
							++net_cnt.listen_;
							break;
						default:
							break;
					}
					++net_cnt.total_;
				}

				return net_cnt;
			}


			/*
			 * get socket inode of process. /proc/<pid>/fd
			 */
			static std::vector<uint64_t> get_socket_inodes(uint32_t pid)
			{
				std::vector<uint64_t> inodes;

				const std::string base_path = Util::make_str("/proc/%lu/fd", pid);
				Dir dir;
				if (!dir.open(base_path.c_str())) {
					Log::err("%s %s", base_path.c_str(), dir.err().c_str());
					return inodes;
				}
				
				struct dirent* subdir = nullptr;
				while ((subdir = dir.readdir())) {

					std::string path = Util::make_str("%s/%s", base_path.c_str(), subdir->d_name);

					struct stat sb;
					if (stat(path.c_str(), &sb) == -1) continue;
					if (!S_ISSOCK(sb.st_mode)) continue;
					inodes.push_back(sb.st_ino);
				}
				return inodes;
			}


		};
	}
}

