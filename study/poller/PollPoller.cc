#include<boost/implicit_cast.hpp>

#include"PollPoller.h"
#include"../Channel.h"
#include"../base/Timestamp.h"
#include<poll.h>
#include<errno.h>

namespace study {

PollPoller::PollPoller(EventLoop* loop) 
    :Poller(loop) {}

PollPoller::~PollPoller() = default;

Timestamp PollPoller::poll(int timeoutMs, ChannelList* activeChannels) {
    int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
    Timestamp now(Timestamp::now());
    if(numEvents > 0) {
        // LOG_TRACE ...
        fillActiveChannels(numEvents, activeChannels);
    } else if (numEvents == 0) {
        // LOG_TRACE ...
    }
    return now;
}

void PollPoller::updateChannel(Channel* channel) {
    Poller::assertInLoopThread();
    // LOG_TRACE ...
    if(channel->index() < 0) {
        assert(channels_.find(channel->fd()) == channels_.end());
        struct pollfd pfd;
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        pollfds_.push_back(pfd);
        int idx = static_cast<int>(pollfds_.size()) - 1;
        channel->set_index(idx);
        channels_[pfd.fd] = channel;
    } else {
        assert(channels_.find(channel->fd()) != channels_.end());
        assert(channels_[channel->fd()] == channel);
        int idx = channel->index();
        assert(idx >= 0 && idx < static_cast<int>(pollfds_.size()));
        struct pollfd& pfd = pollfds_[idx];
        assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd() - 1);
        pfd.fd = channel->fd();
        pfd.events = static_cast<short>(channel->events());
        pfd.revents = 0;
        if(channel->isNoneEvent()) {
            pfd.fd =  -channel->fd() - 1;
        }
    }
}

void PollPoller::removeChannel(Channel* channel) {
    Poller::assertInLoopThread();
    // LOG_TRACE ...
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    assert(channel->isNoneEvent());
    int idx = channel->index();
    assert(idx >= 0 && idx < static_cast<int>(pollfds_.size()));
    const struct pollfd& pfd = pollfds_[idx];
    assert(pfd.fd == -channel->fd() - 1 && pfd.events == channel->events());
    // size_t n = channels_.erase(channel->fd());
    channels_.erase(channel->fd());
    if(boost::implicit_cast<size_t>(idx) == pollfds_.size() - 1) {
        // 如果是在最后一个
        pollfds_.pop_back();
    } else {
        int channelAtEnd = pollfds_.back().fd;
        iter_swap(pollfds_.begin() + idx, pollfds_.end() - 1);
        if(channelAtEnd < 0) {
            channelAtEnd = -channelAtEnd - 1;
        }
        channels_[channelAtEnd]->set_index(idx);
        pollfds_.pop_back();
    }
}

void PollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const {
    for(PollFdList::const_iterator pfd = pollfds_.begin(); pfd != pollfds_.end(); ++pfd) {
        if(pfd->revents > 0) {
            --numEvents;
            ChannelMap::const_iterator ch = channels_.find(pfd->fd);
            assert(ch != channels_.end());
            Channel* channel = ch->second;
            assert(channel->fd() == pfd->fd);
            channel->set_revents(pfd->revents);
            activeChannels->push_back(channel);
        }
    }
}

}