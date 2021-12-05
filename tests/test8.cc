
#include"../study/TcpConnection.h"
#include"../study/TcpServer.h"
#include"../study/eventloop.h"


void onConnection(const study::TcpConnectionPtr& conn) {
    if(conn->connected()) {
        printf("onConnection(): new connection [%s] from %s\n",
            conn->name().c_str(),
            conn->peerAddress().toHostPort().c_str());
    } else {
        printf("onConnection(): new connection [%s] from %s\n",
            conn->name().c_str(),
            conn->peerAddress().toHostPort().c_str());
    }
}

void onMessage(const study::TcpConnectionPtr& conn,
               const char* data,
               ssize_t len) {
    printf("onMessage(): received %zd bytes from connection [%s]\n",
        len, conn->name().c_str());
}

int main() {
    printf("main(): pid = %d\n", getpid());
    study::InetAddress listenAddr(9981);
    study::EventLoop loop;

    study::TcpServer server(&loop, listenAddr);
    server.setConnectionCallback(onConnection);
    server.setMessageCallback(onMessage);
    server.start();

    loop.loop();
}