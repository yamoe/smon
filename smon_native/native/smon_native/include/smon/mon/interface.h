#pragma once

/*
 * only for development rule
 */

namespace smon {


  class TimerReceiver {
  public:
    virtual void on_timer(uint32_t seq) = 0;
  };

  class Timer {
  public:
    Timer() {}
    virtual ~Timer() {}

    virtual void start(TimerReceiver* recvr) = 0;
    virtual void stop() = 0;
  };


  class CPU {
  public:
    CPU() {}
    virtual ~CPU() {}

    virtual void get(CpuData& data) = 0;

  };


  class Mem {
  public:
    Mem() {}
    virtual ~Mem() {}

    virtual void get(MemData& data) = 0;
  };
  
  class Disk {
  public:
    Disk() {}
    virtual ~Disk() {}

    virtual void get(DiskData& data) = 0;
  };


  class Net {
  public:
    Net() {}
    virtual ~Net() {}

    virtual void get(NetData& data) = 0;
  };
  
  
  class Proc {
  public:
    Proc() {}
    virtual ~Proc() {}

    virtual void get(ProcData& data) = 0;

    virtual std::set<std::string> process_list() = 0;
    virtual void regist(const std::string& name) = 0;
    virtual void unregist(const std::string& name) = 0;
  };


  class SYS {
  public:
    SYS() {}
    virtual ~SYS() {}

    virtual SysData get() = 0;
  };


  class Manager {
  public:
    class Callback {
    public:
      virtual void on_callback() = 0;
    };

  public:
    Manager() {}
    virtual ~Manager() {}

    /**
    * @brief 성능 데이터 갱신 주기
    * @detail
    *  성능 데이터 갱신 주기
    * @return void
    */
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void set_period(uint32_t sec) = 0;

    virtual void set_callback(Callback* cb) = 0;
    virtual CpuData cpu() = 0;
    virtual MemData mem() = 0;
    virtual SysData sys() = 0;
    virtual DiskData disk() = 0;
    virtual NetData net() = 0;
    virtual std::set<std::string> proc_list() = 0;
    virtual void regist_proc(const std::string& name) = 0;
    virtual void unregist_proc(const std::string& name) = 0;
    virtual ProcData proc() = 0;

  };

}

