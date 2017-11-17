#pragma once

#include <errno.h>

namespace smon {
	namespace linux {
		class File {
		private:
			FILE* file_ = nullptr;

		public:
			File() {}

			~File()
			{
				if (file_) {
					fclose(file_);
					file_ = nullptr;
				}
			}

			bool open(const char* filename, const char* mode = "r")
			{
				file_ = fopen(filename, mode);
				return (file_ != nullptr);
			}

			std::string err()
			{
				return Util::make_str("%s[%d]", strerror(errno), errno);
			}

			operator FILE* ()
			{
				return file_;
			}

			template <std::size_t size>
			bool get(char(&buf)[size])
			{
				return (fgets(buf, size, file_) != nullptr);
			}

		};
	}
}
