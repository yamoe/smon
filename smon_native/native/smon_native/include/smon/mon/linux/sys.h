#pragma once

#include <ifaddrs.h>
//#include <sys/socket.h>
#include <netdb.h>

#include "file.h"
#include "disk.h"
#include "mem.h"


namespace smon {
  namespace linux {

    class SYS : public smon::SYS {
    public:
      SYS() {}
      virtual ~SYS() override {}

			virtual SysData get() override
			{
				SysData data;

				data.host_name_ = host_name();
				data.os_name_ = os_name();
				data.processor_ = processor();
				data.baseboard_ = baseboard();
				data.ram_ = ram();
				data.disk_ = disk();
				data.ip_ = ip();

				return data;
			}

		private:
			std::string host_name()
			{
				char name[128] = {0,};
				gethostname(name, sizeof(name));
				return name;
			}

			std::string os_name()
			{
				const char* filename = "/etc/os-release";
				File file;
				if (!file.open(filename)) {
					Log::err("%s %s", filename, file.err().c_str());
					return "";
				}

				char line[128] = {0,};
				while (file.get(line)) {
					if (strncmp(line, "PRETTY_NAME=", 12) == 0) {
						return Util::trim(line + 12, " \t\r\n\v\"");
					}
				}
				return "";
			}

			std::string processor()
			{
				const char* filename = "/proc/cpuinfo";
				File file;
				if (!file.open(filename)) {
					Log::err("%s %s", filename, file.err().c_str());
					return "";
				}

				std::string model_name;
				std::string mhz;

				char line[256] = {0,};
				while (file.get(line)) {
					if (strncmp(line, "model name", 10) == 0) {
						model_name = Util::trim(line + 10, " \t\r\n:");
						continue;
					}
					if (strncmp(line, "cpu MHz", 7) == 0) {
						mhz = Util::trim(line + 9, " \t\r\n:");
						continue;
					}

					if (!model_name.empty() && !mhz.empty()) {
						break;
					}
				}

				return Util::make_str("%s %s MHz %u cores",
					model_name.c_str(),
					mhz.c_str(),
					Util::core_count()
				);
			}

			std::string baseboard()
			{
				const char* filename = "/sys/class/dmi/id/board_name";
				File file;
				if (!file.open(filename)) {
					Log::err("%s %s", filename, file.err().c_str());
					return "";
				}

				char line[256] = {0,};
				if (file.get(line)) {
					return Util::trim(line);
				}
				return "";
			}

			std::string ram()
			{
				smon::MemData data;
				smon::linux::Mem().get(data);
				return Util::readable_bytes(data.total_bytes_);
			}

			std::string disk()
			{
				smon::DiskData data;
				smon::linux::Disk().get(data);

				std::string str;
				for (auto it : data.disk_) {
					if (!str.empty()) {
						str += ", ";
					}

					str += Util::make_str("%s(%s)",
						it.first.c_str(),
						Util::readable_bytes(it.second.total_).c_str()
					);
				}
				return str;
			}

			std::string ip()
			{
				struct ifaddrs* ifaddr = nullptr;
				if (getifaddrs(&ifaddr) == -1) {
					return "";
				}

				std::string str;
				char ip[NI_MAXHOST] = {0,};

				for (struct ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
					if (!ifa->ifa_addr) continue;

					int family = ifa->ifa_addr->sa_family;
					if ((family == AF_INET || family == AF_INET6)) {
						int ret = getnameinfo(
							ifa->ifa_addr,
							(family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6),
							ip, NI_MAXHOST, NULL, 0, NI_NUMERICHOST
						);
						if (ret != 0) continue;
						if (!is_external_ip(ip)) continue;

						if (!str.empty()) str += ", ";
						str += ip;

					}
				}

				freeifaddrs(ifaddr);
				return str;
			}

			bool is_external_ip(const char* ip)
			{
				// detect and skip non-external addresses
				if (strcmp(ip, "127.0.0.1") == 0) {
					return false;
				}

				if (strncmp(ip, "fe", 2) == 0) {
					char c = ip[2];
					if (c == '8' || c == '9' || c == 'a' || c == 'b') {
						// link locak
						return false;
					}
					else if (strncmp(ip, "2001:0:", 7) == 0) {
						// special use
						return false;
					}
				}
				return true;
			}
		};

  }
}

