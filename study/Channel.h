#ifndef __STUDY_CHANNEL_H__
#define __STUDY_CHANNEL_H__

#include"base/noncopyable.h"
#include<memory>
#include<functional>

namespace study {

class EventLoop;
class Timestamp;

class Channel : public noncopyable {
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(Timestamp)> ReadEventCallback;
    
    Channel(EventLoop* loop, int fd);
    ~Channel();

    void handleEvent(Timestamp receiveTime);
    void setReadCallback(const ReadEventCallback& cb) {readCallback_ = cb;}
    void setWriteCallback(const EventCallback& cb) {writeCallback_ = cb;}
    void setCloseCallback(const EventCallback& cb) {closeCallback_ = cb;}
    void setErrorCallback(const EventCallback& cb) {errorCallback_ = cb;}

    void tie(const std::shared_ptr<void>&);
    int fd() const {return fd_;}
    int events() const {return events_;}
    void set_revents(int revt) {revents_ = revt;}
    bool isNoneEvent() const {return events_ == kNoneEvent;}

    void enableReading() {events_ |=  kReadEvent; update();}
    void disableReading() {events_ &= ~kReadEvent; update();}
    void enableWriting() {events_ |= kWriteEvent; update();}
    void disableWriting() {events_ &= ~kWriteEvent; update();}
    void disableAll() {events_ = kNoneEvent; update();}

    bool isWriting() const {return events_ & kWriteEvent;}
    bool isReading() const {return events_ & kReadEvent;}


    // for Poller
    int index() {return index_;}
    void set_index(int idx)  {index_ = idx;}

    EventLoop* ownerLoop() {return loop_;}
    
    void remove();

private:
    void update();

    void handleEventWithGuard(Timestamp receiveTime);

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    EventLoop* loop_;
    const int fd_;
    int events_;
    int revents_;
    int index_; // use by poller
    bool logHup_;

    std::weak_ptr<void> tie_;
    bool tied_;
    bool eventHanding_;
    bool addedToLoop_;

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};

}



#endif // _STUDY_CHANNEL_H__