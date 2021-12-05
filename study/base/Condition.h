#ifndef __STUDY_CONDITION_H__
#define __STUDY_CONDITION_H__

#include<pthread.h>

#include"my_mutex.h"




namespace study {

class Condition : noncopyable {
public:
    explicit Condition(MutexLock& mutex)
        :mutex_(mutex) {
        pthread_cond_init(&pcond_, NULL);
    }

    ~Condition() {
        pthread_cond_destroy(&pcond_);
    }

    void wait() {
        MutexLock::UnassignGuard ug(mutex_);    // 在wait释放锁之前将锁的持有者清零
        pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
        //UnassignGuard析构,锁的持有者被设置值为当前线程
    }

    bool waitForSeconds(double seconds);

    void notify() {
        pthread_cond_signal(&pcond_);
    }

    void notifyAll() {
        pthread_cond_broadcast(&pcond_);
    }

private:
    MutexLock& mutex_;
    pthread_cond_t pcond_;
};


}

#endif // __STUDY_CONDITION_H__