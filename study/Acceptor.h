#ifndef __STUDY_ACCEPTOR_H__
#define __STUDY_ACCEPTOR_H__

#include"base/noncopyable.h"
#include"Channel.h"
#include<functional>
#include"Socket.h"

namespace study {

class EventLoop;

class Acceptor : public noncopyable {
public:
    typedef std::function<void(int, const InetAddress&)> NewConnectionCallback;
    Acceptor(EventLoop* loop, const InetAddress& listenAddr, bool reuseport);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb) {
        newConnectionCallback_ = cb;
    }

    bool listenning() const {
        return listenning_;
    }

    void listen();

private:
    void handleRead();

    EventLoop* loop_;
    Socket acceptSocket_;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listenning_;
    int idleFd_;
};


}

#endif // __STUDY_ACCEPTOR_H__
