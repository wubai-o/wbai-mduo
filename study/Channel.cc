#include"Channel.h"
#include<poll.h>
#include"eventloop.h"

namespace study {

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop* loop, int fdArg) 
    :loop_(loop)
    ,fd_(fdArg)
    ,events_(0)
    ,revents_(0)
    ,index_(-1) {

}


//断言在事件处理期间本Channel对象不会析构
Channel::~Channel() {
    assert(!eventHanding_);
}

void Channel::remove() {
    assert(isNoneEvent());
    addedToLoop_ = false;
    loop_->removeChannel(this);
}


void Channel::update() {
    loop_->updateChannel(this);
}

void Channel::handleEvent(Timestamp receiveTime) {
    std::shared_ptr<void> guard;
    if(tied_) {
        guard = tie_.lock();
        if(guard) {
            handleEventWithGuard(receiveTime);
        }
    } else {
        handleEventWithGuard(receiveTime);
    }
}

void Channel::tie(const std::shared_ptr<void>& obj) {
    tie_ = obj;
    tied_ = true;
}

void Channel::handleEventWithGuard(Timestamp receiveTime) {
    eventHanding_ = true;
    // LOG_TRACE << reventsToString();
    if((revents_ & POLLHUP) && !(revents_ & POLLIN)) {
        if(logHup_) {
            // LOG_WARN ...
        }
        if(closeCallback_) closeCallback_();
    }

    if(revents_ & POLLNVAL) {
        // LOG_WARN 
    }

    if(revents_ & (POLLERR | POLLNVAL)) {
        if(errorCallback_) errorCallback_();
    }
    
    if(revents_ & (POLLIN | POLLPRI | POLLRDHUP)) {
        if(readCallback_) readCallback_(receiveTime);
    }

    if(revents_ & POLLOUT) {
        if(writeCallback_) writeCallback_();
    }
    
    eventHanding_ = false;
}


} // namespace study