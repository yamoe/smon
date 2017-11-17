#pragma once

namespace smon {
  namespace win {

    class Timer : public smon::Timer {
    private:
      const DWORD period_msec_ = 1000;
      HANDLE queue_ = nullptr;
      HANDLE timer_ = nullptr;
      TimerReceiver* receiver_ = nullptr;
      uint32_t seq_ = 0;

    public:
      Timer() {}

      virtual ~Timer()
      {
        stop();
      }

      virtual void start(TimerReceiver* receiver) override
      {
        if (queue_) return;

        receiver_ = receiver;

        queue_ = CreateTimerQueue();
        if (!queue_) {
          Log::err("CreateTimerQueue[%u]", GetLastError());
          return;
        }

        BOOL ret = CreateTimerQueueTimer(
          &timer_, queue_,
          Timer::on_timer, this,
          0, period_msec_,
          WT_EXECUTEINTIMERTHREAD
        );

        if (!ret) {
          Log::err("CreateTimerQueueTimer[%u]", GetLastError());
          stop();
          return;
        }
      }

      virtual void stop() override
      {
        if (queue_) {
          if (timer_) {
            DeleteTimerQueueTimer(queue_, timer_, nullptr);
            timer_ = nullptr;
          }
          DeleteTimerQueueEx(queue_, nullptr);
          queue_ = nullptr;
        }
      }

    private:
      void call_on_timer()
      {
        if (receiver_) {
          receiver_->on_timer(seq_++);
        }
      }

      static void CALLBACK on_timer(void* param, BOOLEAN)
      {
        Timer* timer = reinterpret_cast<Timer*>(param);
        timer->call_on_timer();
      }

    };

  }
}