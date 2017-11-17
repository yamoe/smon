#pragma once

#include <string>

#include "file.h"

namespace smon {
	namespace linux {

		class Uptime {
		public:
			Uptime() {}
			~Uptime() {}

			static double get()
			{
				File file;
				if (!file.open("/proc/uptime")) {
					return 0;
				}

				char line[128] = {0,};
				if (!file.get(line)) {
					return 0;
				}

				double uptime = 0;
				double idle = 0;
				sscanf(line, "%lf %lf", &uptime, &idle);
				return uptime;
			}


		};
	}
}
