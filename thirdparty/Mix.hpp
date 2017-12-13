#ifndef MIX_HPP
#define MIX_HPP

#include "systemusrlib.h"
#include "Clock.hpp"
#include "MutexWrap.hpp"
#include <boost/atomic.hpp>

#define LIMIT_RANGE(XMIN, X, XMAX) \
  do {\
    decltype(X) xmin = (XMIN), xmax = (XMAX);\
    CHECK_LE(xmin, xmax);\
    (X) = std::min( (X), xmax );\
    (X) = std::max( (X), xmin );\
  } while (0)

#define GENERATE_CLASS_SINGLETON_GET(CLASSNAME) \
boost::shared_ptr<CLASSNAME> CLASSNAME::Get() { \
  static boost::shared_ptr<CLASSNAME> CLASSNAME##Ptr(new CLASSNAME); \
  return CLASSNAME##Ptr;\
} /*只能在C++11中使用,C++11保证局部静态变量初始化的线程安全*/

template <typename T>
struct DataGuardedManual : public MutexWrap {
  T data;
  DataGuardedManual() : MutexWrap(), data() {}
};

template <typename T>
class DataGuarded {
 public:
  T operator= (T data) {
    boost::lock_guard<boost::mutex> lock(mutex_);
    data_ = data;
    return data;
  }
  operator T() {
    boost::lock_guard<boost::mutex> lock(mutex_);
    return data_;
  }
  DataGuarded() : mutex_(), data_() {} // 显示调用,使data_也能被初始化
 private:
  boost::mutex mutex_;
  T data_;
};

template <typename T>
class DataGuardedDeadzone { // 带死区的数据保护
 public:
  T operator= (T data) {
    boost::lock_guard<boost::mutex> lock(mutex_);
    if (clock_.escaped_us() < deadZoneUS_)
      return data_;
    clock_.reset();
    data_ = data;
    return data;
  }
  operator T() {
    boost::lock_guard<boost::mutex> lock(mutex_);
    return data_;
  }
  DataGuardedDeadzone(double deadZoneUS) : mutex_(), data_(), deadZoneUS_(deadZoneUS), clock_() {}
 private:
  boost::mutex mutex_;
  T data_;
  const double deadZoneUS_;
  Clock clock_;
};

std::string convert_fromBaseType(float  data, int setPrecision);
std::string convert_fromBaseType(double data, int setPrecision);
std::string convert_fromVector(vector<float>);
std::string convert_fromVector(vector<double>);
std::string convert_fromVector(vector<int>);
std::string convert_fromVector(vector<char>);
std::string convert_fromVector(char *buf, int buflen);
template<typename T>
std::string convert_fromVector(vector<T> tvec) {
  stringstream ss;
  for (unsigned idx = 0; idx < tvec.size(); ++idx) {
    if (idx != 0)
      ss << ", ";
    ss << tvec[idx];
  }
  return ss.str();
}
template<typename T>
vector<T> pick_bytype(vector<string> strvec) { // 转换成T型向量
  vector<T> resvec;
  for (auto &&item : strvec) {
    try {
      resvec.push_back(boost::lexical_cast<T>(item));
    } catch (std::exception &e) {
      LOG(INFO) << e.what();
      return vector<T>(); // 直接返回空白向量
    }
  }
  return resvec;
}

#endif //MIX_HPP
