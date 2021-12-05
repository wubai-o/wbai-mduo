#ifndef __STUDY_EVENTLOOPTHREADPOOL_H__
#define __STUDY_EVENTLOOPTHREADPOOL_H__

#include"base/noncopyable.h"

#include<functional>
#include<vector>
#include<string>
#include<memory>

namespace study {

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool : noncopyable {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThreadPool(EventLoop* baseLoop, const std::string& nameArg);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) {numThreads_ = numThreads;}
    void start(const ThreadInitCallback& cb = ThreadInitCallback());

    EventLoop* getNextLoop();
    EventLoop* getNextForHash(size_t hashCode);

    std::vector<EventLoop*> getAllLoops();

    bool started() const {
        return started_;
    }

    const std::string& name() const {
        return name_;
    }
private:
    EventLoop* baseLoop_;
    std::string name_;
    bool started_;
    int numThreads_;
    int next_;
    std::vector<std::unique_ptr<EventLoopThread>> threads_;
    std::vector<EventLoop*> loops_;
};



}


#endif // __STUDY_EVENTLOOPTHREADPOOL_H__