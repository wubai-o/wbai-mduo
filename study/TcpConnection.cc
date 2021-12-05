#include"TcpConnection.h"
#include"Callbacks.h"
#include"InetAddress.h"
#include"Buffer.h"
#include"Channel.h"
#include"Socket.h"
#include"eventloop.h"

#include<iostream>

namespace study {

void net::defaultConnectionCallback(const TcpConnectionPtr& conn) {
    // LOG_TRACE << conn->localAddress().toIpPort() << " -> "
    //        << conn->peerAddress().toIpPort() << " is "
    //        << (conn->connected() ? "UP" : "DOWN");
    std::cout << conn->localAddress().toIpPort() << " -> "
              << conn->peerAddress().toIpPort() << " is " 
              << (conn->connected() ? "UP" : "DOWN") << std::endl;
}

void net::defaultMessageCallback(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Timestamp receiveTime) {
    buffer->retrieveAll();
}

TcpConnection::TcpConnection(EventLoop* loop,
                  const std::string name,
                  int sockfd,
                  const InetAddress& localAddr,
                  const InetAddress& peerAddr) 
    :loop_(loop)
    ,name_(name)
    ,state_(kConnecting)
    ,reading_(true)
    ,socket_(new Socket(sockfd))
    ,channel_(new Channel(loop_, sockfd))
    ,localAddr_(localAddr)
    ,peerAddr_(peerAddr)
    ,highWaterMark_(64 * 1024 * 1024) {
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, _1));
    channel_->setWriteCallback(std::bind(&TcpConnection::handleWrite, this));
    channel_->setCloseCallback(std::bind(&TcpConnection::handleClose, this));
    channel_->setErrorCallback(std::bind(&TcpConnection::handleError, this));

    // LOG_DEBUG ...
    socket_->setKeepAlive(true);
}

    
TcpConnection::~TcpConnection() {
    std::cout << "TcpConnection::dtor[" <<  name_ << "] at " << this
              << " fd=" << channel_->fd()
              << " state=" << stateToString() << std::endl;
}

void TcpConnection::send(const void* message, int len) {
    send(StringPiece(static_cast<const char*>(message), len));
}

void TcpConnection::send(const StringPiece& message) {
    if(state_ == kConnected) {
        if(loop_->isInLoopThread()) {
            sendInLoop(message);
        } else {
            void (TcpConnection::*fp)(const StringPiece&) = &TcpConnection::sendInLoop;
            loop_->runInLoop(std::bind(fp,
                                       this,
                                       message.as_string()));
        }
    }
}

void TcpConnection::send(Buffer* buf) {
    if(state_ == kConnected) {
        if(loop_->isInLoopThread()) {
            sendInLoop(buf->peek(), buf->readableBytes());
            buf->retrieveAll();
        } else {
            void (TcpConnection::*fp)(const StringPiece&) = &TcpConnection::sendInLoop;
            loop_->runInLoop(std::bind(fp,
                                       this,
                                       buf->retrieveAllAsString()));
        }
    }
}

void TcpConnection::sendInLoop(const StringPiece& message) {
    sendInLoop(message.data(), message.size());
}

void TcpConnection::sendInLoop(const void* data, size_t len) {
    loop_->assertInLoopThread();
    ssize_t nwrote = 0;
    size_t remaining = len;
    bool faultError = false;
    if(state_ == kDisconnected) {
        // LOG_WARN
        return;
    }
    if(!channel_->isWriting() && outputBuffer_.readableBytes() == 0) {

        nwrote = sockets::write(channel_->fd(), data, len);
        if(nwrote > 0) {
            remaining = len - nwrote;
            if(remaining == 0 && writeCompleteCallback_) {
                loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
            }
        } else {
            nwrote = 0;
            if(errno != EWOULDBLOCK) {
                // LOG_SYSERR ...
                if(errno == EPIPE || errno == ECONNRESET) {
                    faultError = true;
                }
            }
        }
    }
    assert(remaining <= len);
    if(!faultError && remaining > 0) {
        size_t oldLen = outputBuffer_.readableBytes();
        if(oldLen + remaining >= highWaterMark_ &&
           oldLen < highWaterMark_ && 
           highWaterMarkCallback_) {
            loop_->queueInLoop(std::bind(highWaterMarkCallback_, shared_from_this(), oldLen + highWaterMark_));
        }
        outputBuffer_.append(static_cast<const char*>(data) + nwrote, remaining);
        if(!channel_->isWriting()) {
            channel_->enableWriting();
        }
    }
}


void TcpConnection::shutdown() {
    if(state_ == kConnected) {
        setState(kDisConnecting);
        loop_->runInLoop(std::bind(&TcpConnection::shutdownInLoop, this));
    }
}

void TcpConnection::shutdownInLoop() {
    loop_->assertInLoopThread();
    if(!channel_->isWriting()) {
        socket_->shutdownWrite();
    }
}

const char* TcpConnection::stateToString() const {
    switch(state_) {
        case kDisconnected:
            return "kDisconnected";
        case kConnecting:
            return "kConnecting";
        case kConnected:
            return "kConnected";
        case kDisConnecting:
            return "kDisConnecting";
        default:
            return "unknown state";
    }
}

void TcpConnection::setTcpNoDelay(bool on) {
    socket_->setTcpNoDelay(on);
}

void TcpConnection::connectionEstablished() {
    loop_->assertInLoopThread();
    assert(state_ == kConnecting);
    setState(kConnected);
    channel_->tie(shared_from_this());
    channel_->enableReading();

    connectionCallback_(shared_from_this());
}

void TcpConnection::forceClose() {
    if(state_ == kConnected || state_ == kConnecting) {
        setState(kDisConnecting);
        loop_->queueInLoop(std::bind(&TcpConnection::forceCloseInLoop, shared_from_this()));
    }
}


void TcpConnection::forceCloseInLoop() {
    loop_->assertInLoopThread();
    if(state_ == kConnected || state_ == kConnecting) {
        handleClose();
    }
}


void TcpConnection::connectDestroyed() {
    loop_->assertInLoopThread();
    if(state_ == kConnected) {
        setState(kDisconnected);
        channel_->disableAll();

        connectionCallback_(shared_from_this());
    }
    channel_->remove();
}

void TcpConnection::handleRead(Timestamp receiveTime) {
    loop_->assertInLoopThread();
    int savedErrno = 0;
    ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
    if(n > 0) {
        messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
    } else if(n == 0) {
        handleClose();
    } else {
        errno = savedErrno;
        // LOG_SYSERR << "TcpConnection::handleRead";
        handleError();
    }
}

void TcpConnection::handleWrite() {
    loop_->assertInLoopThread();
    if(channel_->isWriting()) {
        ssize_t n = sockets::write(channel_->fd(),
                                   outputBuffer_.peek(),
                                   outputBuffer_.readableBytes());
        if(n > 0) {
            outputBuffer_.retrieve(n);
            if(outputBuffer_.readableBytes() == 0) {
                // 一旦发送完毕,立刻关闭事件,避免busy loop
                channel_->disableWriting();
                if(writeCompleteCallback_) {
                    loop_->queueInLoop(std::bind(writeCompleteCallback_, shared_from_this()));
                }
                if(state_ == kDisConnecting) {
                    shutdownInLoop();
                }
            } else {
                // LOG_SYSERR ...
            }
        }
    } else {
        // LOG_TRACE ...
    }
}

void TcpConnection::handleClose() {
    loop_->assertInLoopThread();
    // LOG_TRACE ...
    assert(state_ == kConnected || state_ == kDisConnecting);
    setState(kDisconnected);
    channel_->disableAll();

    TcpConnectionPtr guardThis(shared_from_this());
    connectionCallback_(guardThis);
    closeCallback_(guardThis);
}

void TcpConnection::handleError() {
    // int err = sockets::getSocketError(channel_->fd());
    // LOG_ERROR ...
}



}