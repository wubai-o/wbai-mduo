#ifndef __STUDY_EVENTLOOPTHREAD_H__
#define __STUDY_EVENTLOOPTHREAD_H__

#include"base/noncopyable.h"
#include"base/my_mutex.h"
#include"base/Thread.h"
#include"base/Condition.h"

#include<functional>

namespace study {

class EventLoop;

class EventLoopThread : noncopyable {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback(),
                    const std::string& name = std::string());

    ~EventLoopThread();
    EventLoop* startLoop();

private:
    void threadFunc();

    EventLoop* loop_;
    bool exiting_;
    Thread thread_;
    MutexLock mutex_;
    Condition cond_;
    ThreadInitCallback callback_;
};

}

#endif 