#pragma once

#include <stdint.h>
#include <string>

#include "file.h"
#include "../../util/util.h"
#include "../../util/log.h"

namespace smon {
	namespace linux {


		class ProcStat {
			public:
				class Stat {
					public:
						uint32_t th_cnt_ = 0;
						uint64_t pmem_ = 0;
						uint64_t vmem_ = 0;
						uint64_t starttime_ = 0;
						uint64_t utime_ = 0;
						uint64_t stime_ = 0;
						uint64_t cutime_ = 0;
						uint64_t cstime_ = 0;

						/*
						 * reference :
						 *	https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
						 *	http://blog.purewell.biz/2007/12/linux-cpu.html
						 */
						uint32_t cpu(double uptime, Stat& pre)
						{

							if (uptime == 0) return 0;
							if (pre.is_overflow(*this)) return 0;

							uint64_t starttime = starttime_ - pre.starttime_;
							uint64_t utime = utime_ - pre.utime_;
							uint64_t stime = stime_ - pre.stime_;
							uint64_t cutime = cutime_ - pre.cutime_;
							uint64_t cstime = cstime_ - pre.cstime_;

							/*
							 * include the time from children processes
							 * or can use only process time blow
							 * double total_time = utime + stime;
							 */
							double total_time = utime + stime + cutime + cstime;
							double seconds = uptime - (starttime / hertz());

							double percent = total_time / seconds;
							return static_cast<uint32_t>(Util::round(percent, -1));
						}

					private:
						bool is_overflow(Stat& now)
						{
							return (
									now.cutime_ < cutime_ ||
									now.cstime_ < cstime_ ||
									now.starttime_ < starttime_ ||
									now.utime_ < utime_ ||
									now.stime_ < stime_
									);
						}

						double hertz()
						{
							static double hz = sysconf(_SC_CLK_TCK);
							return hz;
						}
				};

			public:
				ProcStat() {}
				~ProcStat() {}

				static Stat get(uint32_t pid)
				{
					Stat stat;

					// http://man7.org/linux/man-pages/man5/proc.5.html
					std::string path = Util::make_str("/proc/%u/stat", pid);

					File file;
					if (!file.open(path.c_str())) {
						Log::err("%s %s", path.c_str(), file.err().c_str());
						return stat;
					}


					char line[1024] = {0,};
					uint64_t dum = 0;
					char dum_char = ' ';
					char comm[128] = {0,};

					while (file.get(line)) {
						sscanf(line,
								"%lu %s %c %lu %lu %lu %lu %lu %lu %lu "
								//  1  2  3  4  5  6  7  8  9  10
								"%lu %lu %lu %lu %lu %lu %lu %lu %lu %u "
								//  11  12  13  14  15  16  17  18  19  20
								"%ld %lu %lu %lu %lu", 
								//	21  22  23  24  25 
								&dum, // 1. pid %d
								comm, // 2. comm %s
								&dum_char, // 3. state %c
								&dum, // 4. ppid %d
								&dum, // 5. pgrp %d
								&dum, // 6. session %d
								&dum, // 7. tty_nr %d
								&dum, // 8. tpgid %d
								&dum, // 9. flags %u
								&dum, // 10. minfit %lu
								&dum, // 11. ominflt %lu
								&dum, // 12. majflt %lu
								&dum, // 13. omjflt %lu
								&stat.utime_, // 14. utime %lu
								&stat.stime_, // 15. stime %lu
								&stat.cutime_, // 16. cutime %ld
								&stat.cstime_, // 17. cstime %ld 
								&dum, // 18. priority %ld 
								&dum, // 19. nice %ld 
								&stat.th_cnt_, // 20. num_threads %ld 
								&dum, // 21. itrealvalue %ld 
								&stat.starttime_, // 22. starttime %lld
								&stat.vmem_, // 23. vsize %lu
								&stat.pmem_, // 24. rss %ld
								&dum  // 25. rsslim %lu
									);
					}

					stat.pmem_ = stat.pmem_ * page_size();
					return stat;
				}

				/*
				 * get page size. generally 4096 bytes
				 */
				static uint64_t page_size()
				{
					static uint64_t page_size = getpagesize();
					return page_size;
				}



		};

	}
}

