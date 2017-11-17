#pragma once

#include <cstring>
#include <dirent.h>
#include <stdint.h>
#include <string>
#include <map>
#include <vector>
#include <set>

#include "dir.h"
#include "file.h"
#include "../../util/log.h"
#include "../../util/util.h"

namespace smon {
	namespace linux {

		class ProcList {
		public:
			typedef std::map<std::string, std::vector<uint32_t>> Map; // key : process name, value : pid
			typedef std::set<std::string> Set; // process name

		public:
			ProcList() {}
			~ProcList() {}


			Map get_map()
			{
				Map map;

				Dir dir;
				if (!dir.open("/proc")) {
					Log::err("/proc %s", dir.err().c_str());
					return map;
				}

				char line[128] = {0,};
				char key[128] = {0,};
				char value[128] = {0,};

				struct dirent* subdir = nullptr;
				while ((subdir = dir.readdir())) {
					uint32_t pid = convert_pid(subdir->d_name);
					if (pid == 0) continue;

					std::string path = Util::make_str("/proc/%s/status", subdir->d_name);

					File file;
					if (!file.open(path.c_str())) continue;
					if (!file.get(line)) continue;

					sscanf(line, "%s %s",  key, value);

					map[value].push_back(pid);
				}
				return map;
			}

			Set get_set()
			{
				Set set;
				for (auto it : get_map()) {
					set.emplace(it.first);
				}
				return set;
			}

		private:
			uint32_t convert_pid(const char* str)
			{
				int len = strlen(str);
				for (int i=0; i<len; ++i) {
					if (str[i] < '0' || str[i] > '9') {
						return 0;
					}
				}
				return atoi(str);
			}
			


		};
	}
}
