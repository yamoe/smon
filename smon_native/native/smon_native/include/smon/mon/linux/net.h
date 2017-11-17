#pragma once

#include <ifaddrs.h>
#include <linux/if_link.h>

namespace smon {
  namespace linux {

    class Net : public smon::Net {
		private:
			std::map<std::string, NetUsage> pre_net_data_;

    public:
      Net() {}
      virtual ~Net() override {}


			virtual void get(NetData& data) override
			{
				data.clear();

				auto names = interfaces_name();

				// or can use /sys/class/net/eth0/statistics/rx_bytes alternatively
				struct ifaddrs* ifaddr = nullptr;

				if (getifaddrs(&ifaddr) == -1) {
					return;
				}

				for (struct ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
					if (!ifa->ifa_addr) {
						continue;
					}

					int family = ifa->ifa_addr->sa_family;
					if (family == AF_PACKET && ifa->ifa_data != NULL) {

						const char* interface_name = ifa->ifa_name;
						if (std::find(names.begin(), names.end(), interface_name) == names.end()) {
							continue;
						}

						struct rtnl_link_stats* stats = reinterpret_cast<rtnl_link_stats*>(ifa->ifa_data);

						//NetUsage& usage = data.net_[ifa->ifa_name];
						NetUsage usage;
						usage.sent_ = stats->tx_bytes;
						usage.recved_ = stats->rx_bytes;
						usage.total_ = usage.sent_ = usage.recved_;

						auto it = pre_net_data_.find(interface_name);
						if (it == pre_net_data_.end()) {
							pre_net_data_[interface_name] = usage;
							continue;
						}

						NetUsage& pre_usage = it->second;
						if (is_overflow(pre_usage, usage)) {
							pre_net_data_.erase(it);
							continue;
						}

						data.net_[interface_name] = minus(usage, pre_usage);
						pre_net_data_[interface_name] = usage;

					}
				}
				freeifaddrs(ifaddr);
			}

		private:
			NetUsage minus(NetUsage& a, NetUsage& b)
			{
				NetUsage r;
				r.total_ = a.total_ - b.total_;
				r.sent_ = a.sent_ - b.sent_;
				r.recved_ = a.recved_ - b.recved_;
				return r;
			}

			bool is_overflow(NetUsage& pre, NetUsage& now)
			{
				return (
					pre.sent_ > now.sent_ ||
					pre.recved_ > now.recved_ ||
					pre.total_ > now.total_
				);
			}

			std::vector<std::string> interfaces_name()
			{
				std::vector<std::string> names;

				struct ifaddrs* ifaddr = nullptr;
				if (getifaddrs(&ifaddr) == -1) {
					return names;
				}

				for (struct ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
					if (!ifa->ifa_addr) {
						continue;
					}
					int family = ifa->ifa_addr->sa_family;
					if ((family == AF_INET || family == AF_INET6)) {
						names.push_back(ifa->ifa_name);
					}
				}

				freeifaddrs(ifaddr);
				return names;
			}
    };

  }
}

