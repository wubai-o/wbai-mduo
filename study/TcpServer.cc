#include"TcpServer.h"
#include"eventloop.h"
#include"InetAddress.h"
#include"Acceptor.h"
#include"EventLoopThreadPool.h"
#include"TcpConnection.h"




namespace study {
TcpServer::TcpServer(EventLoop* loop,
          const InetAddress& listenAddr,
          const std::string& nameArg,
          Option option = kNoReusePort) 
    :loop_(loop)
    ,ipPort_(listenAddr.toIpPort())
    ,name_(nameArg)
    ,acceptor_(new Acceptor(loop_, listenAddr, option == kReusePort))
    ,threadPool_(new EventLoopThreadPool(loop_, name_))
    ,connectionCallback_(net::defaultConnectionCallback)
    ,messageCallback_(net::defaultMessageCallback) {
    acceptor_->setNewConnectionCallback(std::bind(&TcpServer::newConnection, this, _1, _2));
}
    
TcpServer::~TcpServer() {
    loop_->assertInLoopThread();
    // LOG_TRACE ...
    for(auto& item : connections_) {
        TcpConnectionPtr conn(item.second);
        item.second.reset();
        conn->getLoop()->runInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
    }
}

// 0 means all I/O in loop's thread, no thread will create (default value)
// 1 means all I/O in another thread
// N means a thread pool with N threads,
// new connections are assigned on a round-robin basis
void TcpServer::setThreadNum(int numThreads) {
    assert(0 <= numThreads);
    threadPool_->setThreadNum(numThreads);
}

void TcpServer::start() {
    if(started_.getAndSet(1) == 0) {
        threadPool_->start(ThreadInitCallback);
        assert(!acceptor_->listenning());
        loop_->runInLoop(std::bind(&Acceptor::listen, get_pointer(acceptor_)));
    }
}

void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr) {
    loop_->assertInLoopThread();
    EventLoop* ioLoop = threadPool_->getNextLoop();
    char buf[64];
    snprintf(buf, sizeof(buf), "-%s#%d", ipPort_.c_str(), nextConnId_);
    ++nextConnId_;
    std::string connName = name_ + buf;

    std::cout << "TcpServer::newConnection [" << name_
              << "] - new connection [" << connName
              << "] from " << peerAddr.toIpPort() << std::endl;

    InetAddress localAddr(sockets::getLocalAddr(sockfd));
    TcpConnectionPtr conn(new TcpConnection(ioLoop, connName, sockfd, localAddr, peerAddr));

    connections_[connName] = conn;
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->setWriteCompleteCallback(writeCompleteCallback_);
    conn->setCloseCallback(std::bind(&TcpServer::removeConnection, this, _1));
    ioLoop->runInLoop(std::bind(&TcpConnection::connectionEstablished, conn));
}

void TcpServer::removeConnection(const TcpConnectionPtr& conn) {
    loop_->runInLoop(std::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn) {
    loop_->assertInLoopThread();
    std::cout << "TcpServer::removeConnectionInLoop [" << name_ 
              << "] - connection " << conn->name() << std::endl;
    size_t n = connections_.erase(conn->name());
    (void)n;
    assert(n == 1);
    EventLoop* ioLoop = conn->getLoop();
    ioLoop->queueInLoop(std::bind(&TcpConnection::connectDestroyed, conn));
}

}   // namespace study