#ifndef __STUDY_TIMERQUEUE_H__
#define __STUDY_TIMERQUEUE_H__

#include"base/noncopyable.h"
#include"Channel.h"
#include<vector>
#include<memory>
#include<set>
#include"base/Timestamp.h"
#include"Callbacks.h"

namespace study {

class EventLoop;
class TimerId;
class Timer;

class TimerQueue : public noncopyable {
public:
    TimerQueue(EventLoop* loop);
    ~TimerQueue();

    TimerId addTimer(const TimerCallback& cb, Timestamp when, double interval);
    void cancel(TimerId timerId);

private:
    // FIXME: use unique_ptr<Timer> instead of raw pointers
    typedef std::pair<Timestamp, Timer*> Entry;
    typedef std::set<Entry> TimerList;
    typedef std::pair<Timer*, int64_t> ActiveTimer;
    typedef std::set<ActiveTimer> ActiveTimerSet;

    void addTimerInLoop(Timer* timer);
    void cancelInLoop(TimerId timerId);
    // call when timerfd alarms 
    void handleRead();
    // move out all expired timers
    std::vector<Entry> getExpired(Timestamp now);
    void reset(const std::vector<Entry>& expired, Timestamp now);
    bool insert(Timer* timer);

    EventLoop* loop_;
    const int timerfd_;
    Channel timerfdChannel_;
    TimerList timers_;

    // for cancel
    bool callingExpiredTimers_;
    ActiveTimerSet activeTimers_;
    ActiveTimerSet cancelingTimers_;
};






}






#endif