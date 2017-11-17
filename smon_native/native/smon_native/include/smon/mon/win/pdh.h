#pragma once

#include <Pdh.h>
#include <pdhmsg.h>
#pragma comment(lib,"Pdh.lib")


namespace smon {
  namespace win {

    /**
    * @brief Performance Data Helper(PDH) wrapper class
    * @detail 
    * 카운터 추가/조회/삭제 수행
    * 카운터 path 등의 문자열은 대소문자 구분하지 않음
    * https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa373193(v=vs.85).aspx
    */
    class PDH {
    public:


      /**
      * @brief pdh counter 핸들(PDH_HCOUNTER) 를 갖는 클래스
      * @detail
      * 클래스 생성/해제에 자유롭지만
      * 핸들 생성 및 등록은 PDH의 open(), close() 메소드를 통해 수행해야 함
      */
      class Counter {
      private:
        PDH_HCOUNTER h_ = nullptr;

      public:
        Counter() {}

        Counter(PDH_HCOUNTER h) : h_(h) {}

        ~Counter() {}

        PDH_HCOUNTER handle() { return h_; }
        void handle(PDH_HCOUNTER h) { h_ = h; }
      };

    private:
      PDH_HQUERY query_ = nullptr;

    public:
      PDH()
      {
        PDH_STATUS ret = PdhOpenQuery(nullptr, NULL, &query_);
        if (ret != ERROR_SUCCESS) {
          Log::err("PdhOpenQuery[0x%08XL]", ret);
          return;
        }

      }

      ~PDH()
      {
        if (query_) {
          PdhCloseQuery(query_);
          query_ = nullptr;
        }
      }

      /**
      * @brief path를 받아 counter 생성
      * @detail
      * open("\\Processor(_Total)\\% Processor Time") 과 같이 사용
      * @return Counter 객체
      */
      Counter open(const std::string& path)
      {
        PDH_HCOUNTER counter = nullptr;
        PDH_STATUS ret = PdhAddCounterA(query_, path.c_str(), NULL, &counter);
        if (ret != ERROR_SUCCESS) {
          Log::err("PdhAddCounterA[0x%08XL] - %s", ret, path.c_str());
          return Counter(counter);
        }
        return Counter(counter);
      }

      /**
      * @brief 등록된 카운터들의 데이터를 갱신
      */
      void update()
      {
        PDH_STATUS ret = PdhCollectQueryData(query_);
        if (ret != ERROR_SUCCESS) {
          Log::err("PdhCollectQueryData[0x%08XL]", ret);
        }
      }

      template <typename T>
      T get_round(Counter& counter, int round_pos = 0)
      {
        return static_cast<T>(Util::round(get(counter), round_pos));
      }

      template <typename T>
      T get_ceil(Counter& counter)
      {
        return static_cast<T>(std::ceil(get(counter)));
      }

      template <typename T = double>
      T get(Counter& counter)
      {
        PDH_FMT_COUNTERVALUE value = { 0, };
        PDH_STATUS ret = PdhGetFormattedCounterValue(counter.handle(), PDH_FMT_DOUBLE, NULL, &value);
        if (ret != ERROR_SUCCESS) {
          Log::debug("PdhGetFormattedCounterValue[0x%08XL]", ret);
        }
        return static_cast<T>(value.doubleValue);
      }

      void close(Counter& counter)
      {
        PdhRemoveCounter(counter.handle());
        counter.handle(NULL);
      }


      static std::vector<std::string> instance_list_to_lower(const std::string& object_name)
      {
        return instance_list(object_name, false);
      }

      /**
      * @brief PDH 오브젝트의 인스턴스 목록
      * @detail
      * instance_list("Processor") 인 경우 모든 프로세스 이름 목록을 리턴
      * @param std::string object_name 오브젝트 이름
      * @param bool to_lower 소문자 변환 여부
      * @return instance list
      */
      static std::vector<std::string> instance_list(const std::string& object_name, bool to_lower = false)
      {
        std::vector<std::string> list;

        // refresh
        DWORD size = 0;
        PdhEnumObjectsA(NULL, NULL, NULL, &size, PERF_DETAIL_WIZARD, TRUE);

        DWORD counter_size = 0;
        DWORD instance_size = 0;

        PDH_STATUS ret = ERROR_SUCCESS;
        ret = PdhEnumObjectItemsA(
          NULL,
          NULL,
          object_name.c_str(),
          NULL,
          &counter_size,
          NULL,
          &instance_size,
          PERF_DETAIL_WIZARD,
          0
        );

        if (counter_size == 0 || instance_size == 0) {
          return list;
        }

        std::string counter_buf(counter_size, 0x00);
        std::string instance_buf(instance_size, 0x00);
        ret = PdhEnumObjectItemsA(
          NULL,
          NULL,
          object_name.c_str(),
          &counter_buf[0],
          &counter_size,
          &instance_buf[0],
          &instance_size,
          PERF_DETAIL_WIZARD,
          0
        );
        if (ret != ERROR_SUCCESS) {
          Log::err("PdhEnumObjectItemsA[0x%08XL] %s", ret, object_name.c_str());
          return list;
        }

        BYTE* pos = reinterpret_cast<BYTE*>(&instance_buf[0]);
        for (DWORD i = 0; i < instance_size; i += static_cast<DWORD>(strlen(reinterpret_cast<const char*>(pos + i))) + 1) {
          if (pos[i] == 0x00) break;

          std::string instance_name = reinterpret_cast<const char*>(pos + i);
          //if (instance_name.compare("_Total") == 0) continue;

          if (to_lower) {
            list.push_back(Util::to_lower(instance_name));
          }
          else {
            list.push_back(instance_name);
          }
        }
        return list;
      }

      /**
      * @brief instance가 중복되는 경우 pdh 규칙에 따라 instance name 생성
      * @detail
      * pdh는 동일한 이름의 instance가 많은 경우 #index를 붙이지만 0번째는 붙이지 않음
      * make_instance_name("aa", 1) 인 경우 "aa#1"리턴
      * https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa373193(v=vs.85).aspx
      * @return instance name string
      */
      static std::string make_instance_name(const std::string& instance_name, size_t idx)
      {
        if (idx == 0) return instance_name;
        return Util::make_str("%s#%lld", instance_name.c_str(), idx);
      }
    };
  }
}

