#ifndef __STUDY_THREAD_H__
#define __STUDY_THREAD_H__

#include"noncopyable.h"
#include"Atomic.h"
#include"CountDownLatch.h"

#include<pthread.h>
#include<functional>

namespace study {

class Thread : noncopyable {
public:
    typedef std::function<void()> ThreadFunc;

    explicit Thread(ThreadFunc, const std::string& name = std::string());
    ~Thread();

    void start();
    int join();

    bool started() const {return started_;}
    pid_t tid() const {return tid_;}

    const std::string& name() const {return name_;}
    static int numCreate() {return numCreated_.get();}
    
private:
    void setDefaultName();

    bool started_;
    bool joined_;
    pthread_t pthreadId_;
    pid_t tid_;
    ThreadFunc func_;
    std::string name_;
    CountDownLatch latch_;

    static AtomicInt32 numCreated_;
};


}

#endif