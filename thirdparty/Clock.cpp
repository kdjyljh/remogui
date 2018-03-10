#include "Clock.hpp"
#include "systemusrlib.h"

using namespace boost::chrono;

void Clock::reset() {
  initTime_ = system_clock::now();
}

double Clock::escaped_ns() {
  return duration_cast<nanoseconds>(system_clock::now() - initTime_).count();
}

double Clock::escaped_us() {
  return escaped_ns() / 1e3;
}

double Clock::escaped_ms() {
  return escaped_ns() / 1e6;
}

double Clock::escaped_s() {
  return escaped_ns() / 1e9;
}

double global_escaped_ms() {
  static Clock globalClock; // C++11保证局部静态变量初始化的线程安全
  return globalClock.escaped_ms();
}
