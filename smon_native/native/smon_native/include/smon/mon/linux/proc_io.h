#pragma once

#include <stdint.h>
#include <string>

#include "../interface.h"
#include "../../util/util.h"
#include "../../util/log.h"
#include "file.h"

namespace smon {
  namespace linux {
		
		class ProcIO {
		public:
			class IO {
			public:
				uint64_t read_ = 0;
				uint64_t write_ = 0;

				bool is_overflow(IO& now)
				{
					return (
						now.read_ < read_ ||
						now.write_ < write_
					);
				}
			};


		public:
			ProcIO() {}
			~ProcIO() {}

			static IO get(uint32_t pid)
			{
				IO io;

				std::string path = Util::make_str("/proc/%u/io", pid);

				File file;
				if (!file.open(path.c_str())) {
					Log::err("%s %s", path.c_str(), file.err().c_str());
					return io;
				}

				char line[512] = {0,};
				char key[48] = {0,};
				uint64_t val = 0;
				while (file.get(line)) {
					sscanf(line, "%s %lu", key, &val);

					if (strncmp(key, "read_bytes", 10) == 0) {
						io.read_ = val;
					}

					if (strncmp(key, "write_bytes", 11) == 0) {
						io.write_ = val;
					}
				}
				return io;
			}


		};
	}
}

