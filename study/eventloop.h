#ifndef __STUDY_EVENTLOOP_H__
#define __STUDY_EVENTLOOP_H__

#include<assert.h>
#include"base/my_mutex.h"
#include"base/noncopyable.h"
#include"base/CurrentThread.h"
#include"base/Timestamp.h"
#include"TimerId.h"
#include"Callbacks.h"
#include<sys/types.h>
#include<memory>
#include<atomic>
#include<vector>
#include<iostream>

namespace study {

class Poller;
class Channel;
class TimerQueue;

class EventLoop : public noncopyable {
public:
    typedef std::function<void()> Functor;
    EventLoop();
    ~EventLoop();
    
    void loop();
    void quit();

    void updateChannel(Channel* channel);
    void removeChannel(Channel* channel);

    void assertInLoopThread() {
        if (!isInLoopThread()) {
            abortNotInLoopThread();
        }
    }
    
    bool isInLoopThread() const {return threadId_ == CurrentThread::tid();}
    pid_t getThreadId() const {return threadId_;}
    static EventLoop* GetEventLoopOfCurrentThread();
    
    // timerQueue
    TimerId runAt(const Timestamp& time, const TimerCallback& cb);
    TimerId runAfter(double delay, const TimerCallback& cb);
    TimerId runEvery(double interval, const TimerCallback& cb);

    void cancel(TimerId timerId);

    void wakeup();

    void runInLoop(const Functor& cb);
    void queueInLoop(const Functor& cb);

private:
    void abortNotInLoopThread();
    void handleRead(); // weak up
    void doPendingFunctors();

    typedef std::vector<Channel*> ChannelList;

    bool looping_;
    std::atomic<bool> quit_;
    bool callingPendingFunctors_;
    const pid_t threadId_;
    std::unique_ptr<Poller> poller_;
    ChannelList activeChannels_;
    std::unique_ptr<TimerQueue> timerQueue_;
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    mutable MutexLock mutex_;
    std::vector<Functor> pendingFunctors_;
};

}


#endif // __STUDY_EVENTLOOP_H__