#include "MutexWrap.hpp"

MutexWrap::MutexWrap() {
  mutexlocal_ = boost::make_shared<boost::mutex>();
}
void MutexWrap::lock() {
  mutexlocal_->lock();
}
void MutexWrap::unlock() {
  mutexlocal_->unlock();
}
