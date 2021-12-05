#include "../study/eventloop.h"
#include "../study/InetAddress.h"
#include "../study/Socket.h"
#include "../study/Acceptor.h"



void newConnectionCallback(int sockfd, const study::InetAddress& peeraddr) {
    printf("newConnection(): accepted a new connection from %s\n",
        peeraddr.toIpPort().c_str());
    
    ::write(sockfd, "How are you!", 13);
    study::sockets::close(sockfd);
}

int main() {
    printf("main(): pid = %d\n", getpid());
    study::InetAddress listenAddr(3333);
    study::EventLoop loop;

    study::Acceptor acceptor(&loop, listenAddr, false);
    acceptor.setNewConnectionCallback(newConnectionCallback);
    acceptor.listen();

    loop.loop();
    printf("main() end");
}

