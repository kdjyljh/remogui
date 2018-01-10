#ifndef TIMEDTASK_HPP
#define TIMEDTASK_HPP

#include "systemusrlib.h"
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

typedef uint64_t TimedTaskID;
TimedTaskID timed_taskid_gen(uint8_t type, uint64_t idPostfix);

typedef boost::shared_ptr<boost::asio::deadline_timer> DealineTimerPtr;
class TimedTask : public boost::noncopyable {
 public:
  // 登记定时工作的信息. 任务id --> 用于匹配取消定时工作; func --> 将执行的任务;
  // cntRem --> 总执行次数; intervalUS --> 每隔多少时间执行一次,单位ms
  // allowNewTimer --> 表示在workToDo中,当定时器不存在时,是否允许新建该timedTaskID对应的定时器;
  //                   定时器不存在且不能新建意味着定时工作没有成功登记
  void register_work(TimedTaskID timedTaskID, boost::function<void (void)> func, int cntRem, long intervalUS,
                     bool allowNewTimer = true);
  // 将取消所有标记着该帧ID的任务
  void cancel_work(TimedTaskID timedTaskID, bool timeOut = false);

  void work_irrevocable(string funcname, boost::function<void (void)> func, long intervalUS); // 不可撤销的定时任务,任务名称必须唯一

  std::string print();

  static boost::shared_ptr<TimedTask> Get();
 private:
  boost::mutex mutex_;
  boost::shared_ptr<boost::thread> threadp_;

  boost::asio::io_service io_service_;
  boost::unordered_map<TimedTaskID, DealineTimerPtr> workRevocable_;
  boost::unordered_map<string, DealineTimerPtr> workIrrevocable_;

  void signalhandler_revocable(const boost::system::error_code &err,
                               TimedTaskID timedTaskID, boost::function<void (void)> func, int cntRem, long intervalUS);
  void signalhandler_irrevocable(const boost::system::error_code &err,
                                 string funcname, boost::function<void (void)> func, long intervalUS);
  void run();
  TimedTask();
};

// 定时任务参数
extern const uint8_t timedTaskIDPrefix4TimedTask;
extern const uint8_t timedTaskIDPrefix4CommTask;

#endif //TIMEDTASK_HPP
