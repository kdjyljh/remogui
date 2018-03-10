#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include <boost/chrono.hpp>

class Clock {
 public:
  double escaped_ns();
  double escaped_us();
  double escaped_ms();
  double escaped_s();
  void reset();
  Clock() { reset(); }
 private:
  boost::chrono::system_clock::time_point initTime_;
};

double global_escaped_ms();
#define LOGCLOCK(msg) \
LOG(INFO) << "\ttimeescaped: " << global_escaped_ms() << "ms, " << msg

#define BEGIN_TIMER(ACCUM_US_PTR)  \
  shared_ptr<Clock> timerp;\
  if (ACCUM_US_PTR) \
    timerp = boost::make_shared<Clock>()

#define END_TIMER(ACCUM_US_PTR) \
  if (ACCUM_US_PTR) \
    *(ACCUM_US_PTR) += timerp->escaped_us()

#define AVERAGE_TIME(FRAMENUM, ACCUM, AVERAGE) \
  if ((FRAMENUM) % 30 == 0) {\
    (AVERAGE) = (ACCUM) / 30;\
    (ACCUM) = 0;\
  }

#endif // BENCHMARK_HPP
