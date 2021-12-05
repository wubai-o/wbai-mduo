#ifndef __STUDY_SOCKET_H__
#define __STUDY_SOCKET_H__

#include"base/noncopyable.h"
#include"SocketsOps.h"

namespace study {

class InetAddress;

class Socket : public noncopyable {
public:
    explicit Socket(int sockfd)
        :sockfd_(sockfd) {}

    ~Socket();

    int fd() const {return sockfd_;}
    void bindAddress(const InetAddress& localaddr);
    void listen();
    int accept(InetAddress* peeraddr);
    void shutdownWrite();
    void setTcpNoDelay(bool on);
    void setReuseAddr(bool on);
    void setReusePort(bool on);
    void setKeepAlive(bool on);

private:
    const int sockfd_;
};


}


#endif // __STUDY_SOCKET_H__