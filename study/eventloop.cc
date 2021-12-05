#include"eventloop.h"
#include"Channel.h"
#include"Poller.h"
#include"TimerQueue.h"


namespace study {
__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop() 
    :looping_(false)
    ,threadId_(CurrentThread::tid())
    ,poller_(Poller::NewDefaultPoller(this))
    ,timerQueue_(new TimerQueue(this)) {
    // LOG_TRACE ...
    if(t_loopInThisThread) {
        // LOG_FATAL ...
    } else {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop() {
    assert(!looping_);
    t_loopInThisThread = NULL;
}
    
void EventLoop::loop() {
    assert(!looping_);
    assertInLoopThread();
    looping_ = true;
    quit_ = false;
    while(!quit_) {
        activeChannels_.clear();
        poller_->poll(1000, &activeChannels_);
        for(ChannelList::const_iterator it = activeChannels_.begin(); it != activeChannels_.end(); ++it) {
            (*it)->handleEvent(Timestamp::now());
        }
        doPendingFunctors();
    }
    // LOG_TRACE ...
    looping_ = false;
}

void EventLoop::quit() {
    quit_ = true;
    if(!isInLoopThread()) {
        wakeup();
    }
}

void EventLoop::updateChannel(Channel* channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::removeChannel(Channel* channel) {
    assert(channel->ownerLoop() == this);
    assertInLoopThread();
    poller_->removeChannel(channel);
}


EventLoop* EventLoop::GetEventLoopOfCurrentThread() {
    return t_loopInThisThread;
}
    
TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb) {
    return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runAfter(double delay, const TimerCallback& cb) {
    Timestamp time(addTime(Timestamp::now(), delay));
    return timerQueue_->addTimer(cb, time, 0.0);
}

TimerId EventLoop::runEvery(double interval, const TimerCallback& cb) {
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(cb, time, interval);
}

void EventLoop::cancel(TimerId timerId) {
    return timerQueue_->cancel(timerId);
}

void EventLoop::wakeup() {
    //uint64_t one = 1;
    //ssize_t n = sockets::write(wakeupFd_, &one, sizeof(one));
    //if(n != sizeof(one)) {
        // LOG_ERROR ...
    //}/
}

void EventLoop::handleRead() {
    //uint64_t one = 1;
    //ssize_t n = sockets::read(wakeupFd_, &one, sizeof(one));
    //if(n != sizeof(one)) {
        // LOG_ERROR ...
    //}
}

void EventLoop::runInLoop(const Functor& cb) {
    if(isInLoopThread()) {
        cb();
    } else {
        // 如果在其他线程调用,放入任务队列
        queueInLoop(std::move(cb));
    }
}

void EventLoop::queueInLoop(const Functor& cb) {
    {
        MutexLockGuard lock(mutex_);
        pendingFunctors_.push_back(std::move(cb));
    }
    if(!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }
}

void EventLoop::abortNotInLoopThread() {
    printf("abortNotInLoopThread");
}

void EventLoop::doPendingFunctors() {
    std::vector<Functor> functors;
    callingPendingFunctors_ = true;
    {
        MutexLockGuard lock(mutex_);
        functors.swap(pendingFunctors_);
    }

    for(size_t i = 0; i < functors.size(); ++i) {
        functors[i]();
    }
    callingPendingFunctors_ = false;
}





}   // namespace study

