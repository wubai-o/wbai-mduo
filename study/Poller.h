#ifndef __STUDY_POLLER_H__
#define __STUDY_POLLER_H__

#include"base/noncopyable.h"
#include"base/Timestamp.h"
#include"eventloop.h"
#include<vector>
#include<map>

struct pollfd;

namespace study {

class Channle;

class Poller : public noncopyable {
public:
    typedef std::vector<Channel*> ChannelList;

    Poller(EventLoop* loop);
    virtual ~Poller();

    // Polls the I/O events
    // Must be called in the loop thread
    virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

    // Changes the interested I/O events
    // Must be called in the loop thread
    virtual void updateChannel(Channel* channel);
    virtual void removeChannel(Channel* channel) = 0;

    static Poller* NewDefaultPoller(EventLoop* loop);

    void assertInLoopThread() const {
        ownerLoop_->assertInLoopThread();
    }

protected:
    typedef std::vector<struct pollfd> PollFdList;
    typedef std::map<int, Channel*> ChannelMap;

    PollFdList pollfds_;
    ChannelMap channels_;

private:
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;
    
    EventLoop* ownerLoop_;
};





}










#endif // __STUDY_POLLER_H__