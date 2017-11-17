#pragma once

#include <stdint.h>
#include <signal.h>
#include <time.h>
#include "../../util/log.h"

namespace smon {
  namespace linux {

    class Timer : public smon::Timer {
		private:
			const int period_sec_ = 1;
			timer_t timer_id_ = 0;
			TimerReceiver* receiver_ = nullptr;
			uint32_t seq_ = 0;

    public:
      Timer() {}
      virtual ~Timer() override {}


			virtual void start(TimerReceiver* receiver) override 
			{
				if (timer_id_ != 0) return;

				receiver_ = receiver;

				struct sigevent se;
				se.sigev_notify = SIGEV_THREAD;
				se.sigev_value.sival_ptr = this;
				se.sigev_notify_function = Timer::on_timer;
				se.sigev_notify_attributes = NULL;

				if (timer_create(CLOCK_REALTIME, &se, &timer_id_) != 0) {
					timer_id_ = 0;
					Log::err("timer_create");
					return;
				}

				struct itimerspec ts;
				ts.it_value.tv_sec = period_sec_;
				ts.it_value.tv_nsec = 0;
				ts.it_interval.tv_sec = ts.it_value.tv_sec;
				ts.it_interval.tv_nsec = ts.it_value.tv_nsec;

				if (timer_settime(timer_id_, 0, &ts, 0) != 0) {
					timer_delete(timer_id_);
					timer_id_ = 0;
					Log::err("timer_settime");
				}
			}

			virtual void stop() override
			{
				if (timer_id_ == 0) return;
				timer_delete(timer_id_);
				timer_id_ = 0;
			}

		private:
			void call_on_timer()
			{
				if (receiver_) {
					receiver_->on_timer(seq_++);
				}
			}

			static void on_timer(union sigval sv)
			{
				Timer& timer = *reinterpret_cast<Timer*>(sv.sival_ptr);
				timer.call_on_timer();
			}

    };

  }
}
