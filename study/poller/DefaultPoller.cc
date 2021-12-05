#include"../Poller.h"
#include"PollPoller.h"

namespace study {

Poller* Poller::NewDefaultPoller(EventLoop* loop) {
    return new PollPoller(loop);
}

}