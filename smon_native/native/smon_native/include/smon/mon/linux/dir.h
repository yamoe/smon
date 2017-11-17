#pragma once

#include <dirent.h>
#include <string>
#include <error.h>

#include "../../util/util.h"

namespace smon {
	namespace linux {

		class Dir {
		private:
			DIR* dir_ = nullptr;

		public:
			Dir() {}

			~Dir()
			{
				close();
			}

			bool open(const char* filename)
			{
				close();
				dir_ = opendir(filename);
				return (dir_ != nullptr);
			}

			struct dirent* readdir()
			{
				if (!dir_) return nullptr;
				return ::readdir(dir_);
			}
			
			std::string err()
			{
				return Util::make_str("%s[%d]", strerror(errno), errno);
			}

			void close()
			{
				if (dir_) {
					closedir(dir_);
					dir_ = nullptr;
				}
			}

		};
	}
}

