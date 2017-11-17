#pragma once

namespace smon {
  namespace win {

    class SYS : public smon::SYS {
    public:
      SYS() {}
      virtual ~SYS() {}

      virtual SysData get() override
      {
        SysData data;
        data.host_name_ = hostname();
        data.os_name_ = os_name();
        data.processor_ = processor();
        data.baseboard_ = baseboard();
        data.ram_ = ram();
        data.disk_ = disk();
        data.ip_ = ip();

        return data;
      }

      std::string hostname()
      {
        DWORD size = MAX_COMPUTERNAME_LENGTH + 1;
        std::string name(size, 0x00);
        GetComputerNameA(&name[0], &size);
        return name;
      }

      std::string os_name()
      {
        return smon::REG(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", "ProductName").string();
      }

      std::string processor()
      {
        return Util::make_str("%s %u MHz %u cores",
          Util::trim(smon::REG(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "ProcessorNameString").string()).c_str(),
          smon::REG(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", "~MHz").dword(),
          Util::core_count()
        );
      }

      std::string baseboard()
      {
        return smon::REG(HKEY_LOCAL_MACHINE, "HARDWARE\\DESCRIPTION\\System\\BIOS", "BaseBoardProduct").string();
      }

      std::string ram()
      {
        MemData data;
        Mem().get(data);
        return Util::readable_bytes(data.total_bytes_);
      }

      std::string disk()
      {
        std::string str;

        auto drives = WinUtil::drives();
        for (size_t i=0; i<drives.size(); ++i) {
          uint64_t total = 0, use = 0;
          WinUtil::disk_usage(drives[i], total, use);

          str += Util::make_str("%s(%s)",
            drives[i].c_str(),
            Util::readable_bytes(total).c_str()
          );

          if ((i + 1) < drives.size()) {
            str += ", ";
          }
        }

        return str;
      }
      

      std::string ip()
      {
        IP_ADAPTER_ADDRESSES* addrs = nullptr;
        DWORD buf_size = 0;
        DWORD ret = 0;

        ret = ::GetAdaptersAddresses(
          AF_UNSPEC,
          GAA_FLAG_SKIP_ANYCAST |
          GAA_FLAG_SKIP_MULTICAST |
          GAA_FLAG_SKIP_DNS_SERVER |
          GAA_FLAG_SKIP_FRIENDLY_NAME,
          NULL,
          addrs,
          &buf_size);

        if (ret != ERROR_BUFFER_OVERFLOW) {
          Log::err("GetAdaptersAddresses[%d] size", ret);
          return "";
        }

        if (buf_size == 0) {
          return "";
        }

        std::vector<char> buf(buf_size, 0x00);
        addrs = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(&buf[0]);

        ret = ::GetAdaptersAddresses(
          AF_UNSPEC,
          GAA_FLAG_SKIP_ANYCAST |
          GAA_FLAG_SKIP_MULTICAST |
          GAA_FLAG_SKIP_DNS_SERVER |
          GAA_FLAG_SKIP_FRIENDLY_NAME,
          NULL,
          addrs,
          &buf_size);
        if (ret != ERROR_SUCCESS) {
          Log::err("GetAdaptersAddresses[%d]", ret);
          return "";
        }

        std::vector<std::string> ipv;
        for (IP_ADAPTER_ADDRESSES* addr = addrs; addr != NULL; addr = addr->Next) {

          if (addr->IfType == IF_TYPE_SOFTWARE_LOOPBACK) {
            // loopback address
            continue;
          }

          for (IP_ADAPTER_UNICAST_ADDRESS* uaddr = addr->FirstUnicastAddress; uaddr != NULL; uaddr = uaddr->Next) {
            auto family = uaddr->Address.lpSockaddr->sa_family;
            if (family == AF_INET) {
              SOCKADDR_IN* ipv4 = reinterpret_cast<SOCKADDR_IN*>(uaddr->Address.lpSockaddr);

              char ip[INET_ADDRSTRLEN] = { 0 };
              inet_ntop(AF_INET, &(ipv4->sin_addr), ip, INET_ADDRSTRLEN);
              ipv.push_back(ip);
            }
            else if (AF_INET6 == family) {
              SOCKADDR_IN6* ipv6 = reinterpret_cast<SOCKADDR_IN6*>(uaddr->Address.lpSockaddr);

              char ip[INET6_ADDRSTRLEN] = { 0 };
              inet_ntop(AF_INET6, &(ipv6->sin6_addr), ip, INET6_ADDRSTRLEN);

              std::string ipv6_str(ip);

              // Detect and skip non-external addresses
              bool is_link_local = false;
              bool is_special_use = false;

              if (0 == ipv6_str.find("fe")) {
                char c = ipv6_str[2];
                if (c == '8' || c == '9' || c == 'a' || c == 'b') {
                  is_link_local = true;
                }
              }
              else if (0 == ipv6_str.find("2001:0:")) {
                is_special_use = true;
              }

              if (!is_link_local && !is_special_use) {
                ipv.push_back(ip);
              }
            }
          }
        }


        std::string str;
        size_t size = ipv.size();
        for (size_t i = 0; i < size; ++i) {
          str += ipv[i];
          if ((i + 1) != size) {
            str += ", ";
          }
        }
        return str;
      }

    };

  }
}

