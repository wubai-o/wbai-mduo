#include"Acceptor.h"
#include"eventloop.h"
#include"SocketsOps.h"
#include<fcntl.h>
#include"InetAddress.h"

namespace study {
Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport) 
    :loop_(loop)
    ,acceptSocket_(sockets::createNoBlockingOrDie(listenAddr.family()))
    ,acceptChannel_(loop_, acceptSocket_.fd())
    ,listenning_(false)
    ,idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC)) {
    acceptSocket_.setReuseAddr(true);
    acceptSocket_.setReusePort(reuseport);
    acceptSocket_.bindAddress(listenAddr);
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor() {
    acceptChannel_.disableAll();
    acceptChannel_.remove();
    ::close(idleFd_);
}

void Acceptor::listen() {
    loop_->assertInLoopThread();
    listenning_ = true;
    acceptSocket_.listen();
    acceptChannel_.enableReading();
}


void Acceptor::handleRead() {
    loop_->assertInLoopThread();
    InetAddress peerAddr;
    int connfd = acceptSocket_.accept(&peerAddr);
    if(connfd >= 0) {
        if(newConnectionCallback_) {
            newConnectionCallback_(connfd, peerAddr);
        } else {
            sockets::close(connfd);
        }
    } else {
        // LOG_SYSERR ...
        if(errno == EMFILE) {
            ::close(idleFd_);
            idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
            ::close(idleFd_);
            idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
}



}   // namespace study