#ifndef __STUDY_ASYNCLOGGING_H__
#define __STUDY_ASYNCLOGGING_H__

#include"noncopyable.h"
#include"LogStream.h"
#include"Thread.h"
#include"CountDownLatch.h"
#include"my_mutex.h"
#include"Condition.h"


#include<vector>
#include<memory>
#include<atomic>
#include<string>

namespace study {

class AsyncLogging : noncopyable {
public:
    AsyncLogging(const std::string& basename,
                 off_t rollSize,
                 int flushInterval = 3);
    
    ~AsyncLogging() {
        if(running_) {
            stop();
        }
    }

    void append(const char* logline, int len);

    void start() {
        running_ = true;
        thread_.start();
        latch_.wait();
    }

    void stop() {
        running_ = false;
        cond.notify();
        thread_.join();
    }

private:
    void threadFunc();

    typedef study::detail::FixedBuffer<study::detail::kLargeBuffer> Buffer;
    typedef std::vector<std::unique_ptr<Buffer> > BufferVector;
    typedef BufferVector::value_type BufferPtr;

    const int flushInterval_;
    std::atomic<bool> running_;
    const std::string basename_;
    const off_t rollSize_;
    Thread thread_;
    CountDownLatch latch_;
    MutexLock mutex_;
    Condition cond_; // GUARDED_BY(mutex_)
    BufferPtr currentBuffer_; // GUARDED_BY(mutex_)
    BufferPtr nextBuffer_; // GUARDED_BY(mutex_)
    BufferVector buffers_; // GUARDED_BY(mutex_)


};


}   // namespace study

#endif