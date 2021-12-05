#include<sys/timerfd.h>
#include"../study/eventloop.h"
#include"../study/Channel.h"
#include"../study/base/Timestamp.h"
#include<string.h>
#include<unistd.h>
#include<iostream>

study::EventLoop* g_loop;

void timeout(study::Timestamp timestamp) {
    printf("Timeout!\n");
    g_loop->quit();
}


// no PollPoller

int main() {
    study::EventLoop loop;
    g_loop = &loop;

    int timerfd = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    study::Channel channel(&loop, timerfd);
    channel.setReadCallback(std::bind(timeout, study::Timestamp::now()));
    channel.enableReading();
    struct itimerspec howlong;
    bzero(&howlong, sizeof(howlong));
    howlong.it_value.tv_sec = 5;
    ::timerfd_settime(timerfd, 0, &howlong, NULL);
    loop.loop();
    ::close(timerfd);
}