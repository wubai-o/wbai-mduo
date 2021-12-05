#ifndef __STUDY_POLLPOLLER_H__
#define __STUDY_POLLPOLLER_H__

#include"../eventloop.h"
#include"../Poller.h"
#include<vector>


namespace study {

class PollPoller : public Poller {
public:
    PollPoller(EventLoop* loop);
    ~PollPoller() override;

    Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;

    void updateChannel(Channel* channel) override;
    void removeChannel(Channel* channel) override;

private:
    void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

    typedef std::vector<struct pollfd> PollFdList;
    PollFdList pollfds_;
};

}

#endif 