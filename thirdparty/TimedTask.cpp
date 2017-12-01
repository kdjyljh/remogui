#include "TimedTask.hpp"
#include "Mix.hpp"

TimedTaskID timed_taskid_gen(uint8_t type, uint64_t idPostfix) {
  uint64_t mask4Postfix = ~ (static_cast<uint64_t>(0xFF) << 56);
  // 虽然参数idPostfix可能来自网络,但该参数始终在本地组装,因此可以CHECK
  CHECK((idPostfix & mask4Postfix) == idPostfix);
  uint64_t idPrefix = static_cast<uint64_t>(type) << 56;
  return idPrefix | idPostfix;
}

// TimedTask
void TimedTask::register_work(TimedTaskID timedTaskID, boost::function<void (void)> func, int cntRem, long intervalUS,
                              bool allowNewTimer) { // 加锁
  if (cntRem <= 0) // 剩余执行次数至少需要1次,否则直接返回
    return;

  auto handler = boost::bind(&TimedTask::signalhandler_revocable, this,
                             boost::asio::placeholders::error, timedTaskID, func, cntRem, intervalUS);

  boost::lock_guard<boost::mutex> lock(mutex_);
  if (workRevocable_.find(timedTaskID) == workRevocable_.end()) {
    if (! allowNewTimer) // 不允许新建定时器则直接退出
      return;
    workRevocable_[timedTaskID] = boost::make_shared<boost::asio::deadline_timer>(io_service_); // 生成定时器
  }
  // 现在workToDo_[protocolID]一定有效
  workRevocable_[timedTaskID]->expires_from_now(boost::posix_time::microseconds(intervalUS));
  workRevocable_[timedTaskID]->async_wait(handler);
}

void TimedTask::work_irrevocable(string funcname, boost::function<void(void)> func, long intervalUS) {
  boost::lock_guard<boost::mutex> lock(mutex_);
  if (workIrrevocable_.find(funcname) != workIrrevocable_.end()) {
    LOG(INFO) << "ERROR: in TimedTask " << funcname << " has been registed already";
    return;
  }
  workIrrevocable_[funcname] = boost::make_shared<boost::asio::deadline_timer>(io_service_); // 生成定时器

  auto handler = boost::bind(&TimedTask::signalhandler_irrevocable, this,
                             boost::asio::placeholders::error, funcname, func, intervalUS);
  workIrrevocable_[funcname]->expires_from_now(boost::posix_time::microseconds(intervalUS));
  workIrrevocable_[funcname]->async_wait(handler);
}

void TimedTask::signalhandler_revocable(const boost::system::error_code &err,
                                        TimedTaskID timedTaskID, boost::function<void (void)> func, int cntRem, long intervalUS) {
  if (err) {
    if (err != boost::asio::error::operation_aborted)
      LOG(INFO) << "error in signalhandler_revocable: " << err.message();
    return;
  }
  // 因为通过register_work注册得到,所以必定cntRem >= 1
  CHECK(cntRem >= 1);
  func(); cntRem--; // 执行一次,同时剩余次数减一
  if (cntRem >= 1) {
    register_work(timedTaskID, func, cntRem, intervalUS, false); // 递归注册,但不允许新建定时器,因为signalhandler可能会推迟到cancel_work之后执行
  } else { // 该任务已经结束
//    LOG(FATAL) << "通信失败多次，直接退出: " << timedTaskID << " with prefix = " << (timedTaskID >> 56);
    cancel_work(timedTaskID);
  }
}

void TimedTask::signalhandler_irrevocable(const boost::system::error_code &err,
                                          string funcname, boost::function<void(void)> func, long intervalUS) {
  if (err) {
    LOG(INFO) << "error in signalhandler_irrevocable: " << err.message();
    return;
  }

  func(); // 先执行后注册,会延长实际两次操作之间的时间间隔,但是不会出现一直忙碌的情况

  auto handler = boost::bind(&TimedTask::signalhandler_irrevocable, this,
                             boost::asio::placeholders::error, funcname, func, intervalUS);

  boost::lock_guard<boost::mutex> lock(mutex_);
  auto it = workIrrevocable_.find(funcname);
  CHECK(it != workIrrevocable_.end());
  it->second->expires_from_now(boost::posix_time::microseconds(intervalUS));
  it->second->async_wait(handler);
}

void TimedTask::cancel_work(TimedTaskID timedTaskID) { // 加锁
  boost::lock_guard<boost::mutex> lock(mutex_);
  auto workp = workRevocable_.find(timedTaskID);
  if (workp != workRevocable_.end()) { // 找到了
    boost::system::error_code err;
    workp->second->cancel(err);
    workRevocable_.erase(workp);
    if (err)
      LOG(INFO) << "error in cancel_work: " << err.message() << ", timedTaskID = " << timedTaskID;
    // 因为该函数加锁,所以不能调用同样加锁的print()
  }
}

std::string TimedTask::print() { // 加锁,保护workToDo
  std::stringstream ss;
  boost::lock_guard<boost::mutex> lock(mutex_);
  ss << "workRevocable len = " << workRevocable_.size() << ", IDs = ";
  for (auto it = workRevocable_.begin(); it != workRevocable_.end(); ++it) {
    if (it != workRevocable_.begin())
      ss << ", ";
    ss << it->first;
  }
  ss << "\n";
  ss << "workIrrevocable len = "  << workIrrevocable_.size() << ", IDs = ";
  for (auto it = workIrrevocable_.begin(); it != workIrrevocable_.end(); ++it) {
    if (it != workIrrevocable_.begin())
      ss << ", ";
    ss << it->first;
  }
  return ss.str();
}

void TimedTask::run() {
  boost::asio::io_service::work work(io_service_);
  io_service_.run();
}

TimedTask::TimedTask() {
  LOG(INFO) << "CommTimedTask ctor";

  threadp_ = boost::make_shared<boost::thread>(&TimedTask::run, this);
}
GENERATE_CLASS_SINGLETON_GET(TimedTask)

// 定时任务参数, timedTaskIDPrefix4* 代表TimedTaskID这个8字节整数的首字节,因此TimedTaskID剩余可变部分还有7个字节.
const uint8_t timedTaskIDPrefix4TimedTask   = 0x01; // 用于定时器自身状态打印的定时任务
const uint8_t timedTaskIDPrefix4CommTask    = 0x02; // 用于通信失败自动重发的定时任务
