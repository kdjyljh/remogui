#ifndef MUTEXWRAP_HPP
#define MUTEXWRAP_HPP

#include "systemusrlib.h"

class MutexWrap {
 public:
  MutexWrap();
  void lock();
  void unlock();
 protected:
  boost::shared_ptr<boost::mutex> mutexlocal_;
};

#endif //MUTEXWRAP_HPP
