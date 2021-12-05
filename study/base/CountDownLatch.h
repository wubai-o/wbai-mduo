#ifndef __STUDY_COUNTDOWNLATCH_H__
#define __STUDY_COUNTDOWNLATCH_H__

#include"Condition.h"
#include"noncopyable.h"
#include"my_mutex.h"

namespace study {

class CountDownLatch : noncopyable {
public:
    explicit CountDownLatch(int count);

    void wait();

    void countDown();

    int getCount() const;
    
private:
    mutable MutexLock mutex_;
    Condition condition_;
    int count_;
};










}






#endif