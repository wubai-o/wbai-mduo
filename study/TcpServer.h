#ifndef __STUDY_TCPSERVER_H__
#define __STUDY_TCPSERVER_H__

#include"base/noncopyable.h"
#include"Callbacks.h"
#include"base/Atomic.h"

#include<functional>
#include<string>
#include<map>
#include<memory>



namespace study {

class EventLoop;
class EventLoopThreadPool;
class InetAddress;
class Acceptor;

class TcpServer : noncopyable {
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;
    enum Option {
        kNoReusePort,
        kReusePort,
    };

    TcpServer(EventLoop* loop,
              const InetAddress& listenAddr,
              const std::string& nameArg,
              Option option = kNoReusePort);
    
    ~TcpServer();

    const std::string& ipPort() const {return ipPort_;}
    const std::string& name() const {return name_;}
    EventLoop* getLoop() const {return loop_;}

    void setThreadNum(int numThreads);
    void setThreadInitCallback(const ThreadInitCallback& cb) {
        threadInitCallback_ = cb;
    }

    std::shared_ptr<EventLoopThreadPool> threadPool() {
        return threadPool_;
    }

    void start();

    void setConnectionCallback(const ConnectionCallback& cb) {
        connectionCallback_ = cb;
    }

    void setMessageCallback(const MessageCallback& cb) {
        messageCallback_ = cb;
    }

private:
    void newConnection(int sockfd, const InetAddress& peerAddr);
    void removeConnection(const TcpConnectionPtr& conn);
    void removeConnectionInLoop(const TcpConnectionPtr& conn);

    typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

    EventLoop* loop_;
    const std::string ipPort_;
    const std::string name_;
    std::unique_ptr<Acceptor> acceptor_;
    std::shared_ptr<EventLoopThreadPool> threadPool_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    ThreadInitCallback threadInitCallback_;
    AtomicInt32 started_;
    int nextConnId_;
    ConnectionMap connections_;
};

}



#endif // __STUDY_TCPSERVER_H__