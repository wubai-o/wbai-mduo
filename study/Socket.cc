#include"Socket.h"
#include"InetAddress.h"
#include"base/Types.h"
#include<netinet/in.h>
#include<netinet/tcp.h>

namespace study {

Socket::~Socket() {
    sockets::close(sockfd_);
}

void Socket::bindAddress(const InetAddress& localaddr) {
    sockets::bindOrDie(sockfd_, localaddr.getSockAddr());
}

void Socket::listen() {
    sockets::listenOrDie(sockfd_);
}

int Socket::accept(InetAddress* peeraddr) {
    struct sockaddr_in6 addr;
    memZero(&addr, sizeof(addr));
    int connfd = sockets::accept(sockfd_, &addr);
    if(connfd >=  0) {
        peeraddr->setSockAddrInet6(addr);
    }
    return connfd;
}

void Socket::shutdownWrite() {
    sockets::shutdownWrite(sockfd_);
}

void Socket::setTcpNoDelay(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, IPPROTO_TCP, TCP_NODELAY, 
                 &optval,static_cast<socklen_t>(sizeof(optval)));
    // FIXME CHECK
}

void Socket::setReuseAddr(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, 
                 &optval,static_cast<socklen_t>(sizeof(optval)));
}

void Socket::setReusePort(bool on) {
#ifdef SO_REUSEPORT
    int optval = on ? 1 : 0;
    int ret = ::setsockopt(sockfd_, SOL_SOCKET, SO_REUSEPORT, 
                           &optval, static_cast<socklen_t>(sizeof(optval)));
    if(ret < 0 && on) {
        // LOG_SYSERR ...
    }
#else
    if(on) {
        // LOG_ERROR ...
    }
#endif
}

void Socket::setKeepAlive(bool on) {
    int optval = on ? 1 : 0;
    ::setsockopt(sockfd_, SOL_SOCKET, SO_KEEPALIVE,
                 &optval, static_cast<socklen_t>(sizeof(optval)));
    // FIXME CHECK
}


}