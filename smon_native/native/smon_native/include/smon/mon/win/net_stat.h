#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <iphlpapi.h>
#pragma comment(lib, "iphlpapi.lib")


namespace smon {
  namespace win {


    /**
    * @brief tcp, udp 개수 카운트 클래스
    * @detail
    * tcp, udp 목록을 구한 후 process id 가 보유하고 있는 개수를 돌려줌
    */
    class NetStat {
    public:
      class Count {
      public:
        uint32_t tcp_cnt_ = 0;
        uint32_t tcp_listen_cnt_ = 0;
        uint32_t tcp_estab_cnt_ = 0;
        uint32_t udp_cnt_ = 0;
      };
    private:
      // key : pid
      std::map<uint32_t, Count> tbl_;

    public:
      NetStat() {}
      ~NetStat() {}

      void refresh()
      {
        tbl_.clear();
        count_tcp4();
        count_tcp6();
        count_udp4();
        count_udp6();
      }

      Count get(uint32_t pid)
      {
        auto it = tbl_.find(pid);
        if (it == tbl_.end()) {
          return Count();
        }
        return it->second;
      }

    private:
      void count_tcp4()
      {
        ULONG size = 0;
        ULONG ret = NO_ERROR;
        ret = GetTcpTable2(NULL, &size, FALSE);
        if (ret != ERROR_INSUFFICIENT_BUFFER) {
          Log::err("GetTcpTable2[%u] size", ret);
          return;
        }

        std::unique_ptr<MIB_TCPTABLE2> tbl(reinterpret_cast<MIB_TCPTABLE2*>(new char[size]));
        ret = GetTcpTable2(tbl.get(), &size, FALSE);
        if (ret != NO_ERROR) {
          Log::err("GetTcpTable2[%u]", ret);
          return;
        }

        uint32_t esta_cnt = 0;
        for (DWORD i = 0; i < tbl->dwNumEntries; ++i) {

          auto& ctr = tbl_[tbl->table[i].dwOwningPid];

          switch (tbl->table[i].dwState) {
          case MIB_TCP_STATE_LISTEN:
            ++ctr.tcp_listen_cnt_;
            ++ctr.tcp_cnt_;
            break;
          case MIB_TCP_STATE_ESTAB:
            ++ctr.tcp_estab_cnt_;
            ++ctr.tcp_cnt_;
            break;
          default:
            ++ctr.tcp_cnt_;
          }
        }
      }

      void count_tcp6()
      {
        ULONG size = 0;
        ULONG ret = NO_ERROR;
        ret = GetTcp6Table2(NULL, &size, FALSE);
        if (ret != ERROR_INSUFFICIENT_BUFFER) {
          Log::err("GetTcp6Table2[%u] size", ret);
          return;
        }

        std::unique_ptr<MIB_TCP6TABLE2> tbl(reinterpret_cast<MIB_TCP6TABLE2*>(new char[size]));
        ret = GetTcp6Table2(tbl.get(), &size, FALSE);
        if (ret != NO_ERROR) {
          Log::err("GetTcp6Table2[%u]", ret);
          return;
        }

        for (DWORD i = 0; i < tbl->dwNumEntries; ++i) {
          auto& ctr = tbl_[tbl->table[i].dwOwningPid];

          switch (tbl->table[i].State) {
          case MIB_TCP_STATE_LISTEN:
            ++ctr.tcp_listen_cnt_;
            ++ctr.tcp_cnt_;
            break;
          case MIB_TCP_STATE_ESTAB:
            ++ctr.tcp_estab_cnt_;
            ++ctr.tcp_cnt_;
            break;
          default:
            ++ctr.tcp_cnt_;
          }

        }
      }

      void count_udp4()
      {
        DWORD size = 0;
        DWORD ret = NO_ERROR;

        ret = GetExtendedUdpTable(NULL, &size, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0);
        if (ret != ERROR_INSUFFICIENT_BUFFER) {
          Log::err("GetExtendedUdpTable AF_INET[%u] size", ret);
          return;
        }

        std::unique_ptr<MIB_UDPTABLE_OWNER_PID> tbl(reinterpret_cast<MIB_UDPTABLE_OWNER_PID*>(new char[size]));
        ret = GetExtendedUdpTable(tbl.get(), &size, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0);
        if (ret != NO_ERROR) {
          Log::err("GetExtendedUdpTable AF_INET[%u]", ret);
          return;
        }

        for (DWORD i = 0; i < tbl->dwNumEntries; ++i) {
          auto& ctr = tbl_[tbl->table[i].dwOwningPid];
          ++ctr.udp_cnt_;
        }
      }

      void count_udp6()
      {
        DWORD size = 0;
        DWORD ret = NO_ERROR;

        ret = GetExtendedUdpTable(NULL, &size, FALSE, AF_INET6, UDP_TABLE_OWNER_PID, 0);
        if (ret != ERROR_INSUFFICIENT_BUFFER) {
          Log::err("GetExtendedUdpTable AF_INET6[%u] size", ret);
          return;
        }

        std::unique_ptr<MIB_UDPTABLE_OWNER_PID> tbl(reinterpret_cast<MIB_UDPTABLE_OWNER_PID*>(new char[size]));
        ret = GetExtendedUdpTable(tbl.get(), &size, FALSE, AF_INET6, UDP_TABLE_OWNER_PID, 0);
        if (ret != NO_ERROR) {
          Log::err("GetExtendedUdpTable AF_INET6[%u]", ret);
          return;
        }

        for (DWORD i = 0; i < tbl->dwNumEntries; ++i) {
          auto& ctr = tbl_[tbl->table[i].dwOwningPid];
          ++ctr.udp_cnt_;
        }
      }


    };
  }
}