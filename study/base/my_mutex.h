#ifndef __WUBAI_MY_MUTEX_H__
#define __WUBAI_MY_MUTEX_H__

#include<thread>
#include<pthread.h>
#include<memory>
#include<assert.h>

#include"noncopyable.h"
#include"CurrentThread.h"

namespace study {

class MutexLock : noncopyable {
public:
    MutexLock() 
        :holder_(0) {
        pthread_mutex_init(&mutex_, NULL);
    }

    ~MutexLock() {
        assert(holder_ == 0);
        pthread_mutex_destroy(&mutex_);
    }

    bool isLockByThisThread() const {
        return holder_ == CurrentThread::tid();
    }

    void assertLocked() const {
        assert(isLockByThisThread());
    }

    void lock() {
        pthread_mutex_lock(&mutex_);
        assignHolder();
    }
    void unlock() {
        unassignHolder();
        pthread_mutex_unlock(&mutex_);
    }

    pthread_mutex_t* getPthreadMutex() {
        return &mutex_;
    }

private:
    friend class Condition;

    class UnassignGuard : noncopyable {
    public:
        explicit UnassignGuard(MutexLock& owner)
            :owner_(owner) {
            owner_.unassignHolder();
        }

        ~UnassignGuard() {
            owner_.assignHolder();
        }

    private:
        MutexLock& owner_;
    };

    void unassignHolder() {
        holder_ = 0;
    }

    void assignHolder() {
        holder_ = CurrentThread::tid();
    }

    pthread_mutex_t mutex_;
    pid_t holder_;
};

class MutexLockGuard {
public:
    MutexLockGuard(MutexLock& mutex) 
        :mutex_(mutex){
        mutex_.lock();
    }

    ~MutexLockGuard() {
        unlock();
    }

    void lock() {
        mutex_.lock();
    }
    void unlock() {
        mutex_.unlock();
    }
private:
    MutexLock& mutex_;
};



}
#endif