#ifndef __STUDY_TCPCLIENT_H__
#define __STUDY_TCPCLIENT_H__

#include"base/noncopyable.h"
#include"eventloop.h"
#include"Callbacks.h"


#include<memory>
#include<string>

namespace study {

class InetAddress;
class Connector;
typedef std::shared_ptr<Connector> ConnectorPtr;

class TcpClient : noncopyable {
public:
    TcpClient(EventLoop* loop,
              const InetAddress& serverAddr,
              const std::string& nameArg);
    
    ~TcpClient();

    void connect();
    void disconnect();
    void stop();

    TcpConnectionPtr connection() const {
        MutexLockGuard lock(mutex_);
        return connection_;
    }

    EventLoop* getLoop() const {return loop_;}
    bool retry() const {return retry_;}
    void enableRetry() {retry_ = true;}

    const std::string& name() const {return name_;}
    void setConnectionCallback(ConnectionCallback cb) {
        connectionCallback_ = cb;
    }

    void setMessageCallback(MessageCallback cb) {
        messageCallback_ = cb;
    }

    void writeCompleteCallback(WriteCompleteCallback cb) {
        writeCompleteCallback_ = cb;
    }

private:

    void newConnection(int sockfd);
    void removeConnection(const TcpConnectionPtr& conn);

    EventLoop* loop_;
    ConnectorPtr connector_;
    const std::string name_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    bool retry_;
    bool connect_;
    int nextConnId_;
    mutable MutexLock mutex_;
    TcpConnectionPtr connection_;
};

}


#endif // __STUDY_TCPCLIENT_H__