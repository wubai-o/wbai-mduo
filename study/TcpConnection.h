#ifndef __WUBAI_TCP_CONNECTION_H__
#define __WUBAI_TCP_CONNECTION_H__

#include"base/noncopyable.h"
#include"Callbacks.h"
#include"base/Types.h"
#include"Buffer.h"
#include"InetAddress.h"

#include<string>
#include<memory>
#include<boost/any.hpp>

namespace study {

class EventLoop;
class Socket;
class Channel;

class TcpConnection :public noncopyable,public std::enable_shared_from_this<TcpConnection> {
public:
    typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
    
    TcpConnection(EventLoop* loop,
                  const std::string name,
                  int sockfd,
                  const InetAddress& localAddr,
                  const InetAddress& peerAddr);

    ~TcpConnection();

    EventLoop* getLoop() const {return loop_;}
    const std::string& name() const {return name_;}
    const InetAddress& localAddress() const {return localAddr_;}
    const InetAddress& peerAddress() const {return peerAddr_;}
    bool connected() const {return state_ == kConnected;}
    bool disconnected() const {return state_ == kDisconnected;}
    // return true if success
    bool getTcpInfo(struct tcp_info*) const;

    // void send(std::string&& message); // C++11
    void send(const void* message, int len);
    void send(const StringPiece& message);

    // void send(Buffer&& message); // C++11
    void send(Buffer* buf);
    void shutdown();

    // void
    void forceClose();
    void forceCloseWithDelay(double seconds);
    void setTcpNoDelay(bool on);

    void startRead();
    void stopRead();
    bool isReading() const {return reading_;}

    void setContext(const boost::any& context) {
        context_ = context;
    }

    const boost::any& getContext() const {
        return context_;
    }

    boost::any* getMutableContext() {
        return &context_;
    }

    void setConnectionCallback(const ConnectionCallback& cb) {
        connectionCallback_ = cb;
    }

    void setMessageCallback(const MessageCallback& cb) {
        messageCallback_ = cb;
    }

    void setWriteCompleteCallback(const WriteCompleteCallback& cb) {
        writeCompleteCallback_ = cb;
    }

    void setHighWateMarkCallback(const HighWaterMarkCallback& cb) {
        highWaterMarkCallback_ = cb;
    }

    Buffer* inputBuffer() {
        return &inputBuffer_;
    }

    Buffer* outputBuffer() {
        return &outputBuffer_;
    }

    void setCloseCallback(const CloseCallback& cb) {
        closeCallback_ = cb;
    }

    void connectionEstablished();

    void connectDestroyed();

private:
    enum StateE {
        kDisconnected,
        kConnecting,
        kConnected,
        kDisConnecting,
    };

    void handleRead(Timestamp receiveTime);
    void handleWrite();
    void handleClose();
    void handleError();
    void sendInLoop(const StringPiece& message);
    void sendInLoop(const void* message, size_t len);
    void shutdownInLoop();

    void forceCloseInLoop();
    void setState(StateE s) {state_ = s;}
    const char* stateToString() const;
    void startReadInLoop();
    void stopReadInLoop();

    EventLoop* loop_;
    const std::string name_;
    StateE state_;
    bool reading_;
    std::unique_ptr<Socket> socket_;
    std::unique_ptr<Channel> channel_;
    const InetAddress localAddr_;
    const InetAddress peerAddr_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    WriteCompleteCallback writeCompleteCallback_;
    HighWaterMarkCallback highWaterMarkCallback_;
    CloseCallback closeCallback_;
    size_t highWaterMark_;
    Buffer inputBuffer_;
    Buffer outputBuffer_;

    boost::any context_;
};

}

#endif // __WUBAI_TCP_CONNECTION_H__