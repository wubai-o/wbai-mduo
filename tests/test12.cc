#include"../study/eventloop.h"
#include"../study/InetAddress.h"
#include"../study/Connector.h"


study::EventLoop* g_loop;


void connectionCallback(int sockfd) {
    printf("connectioned. \n");
    g_loop->quit();
}


int main(int argc, char** argv) {
    study::EventLoop loop;
    g_loop = &loop;
    study::InetAddress addr("127.0.0.1", 9981);
    study::Connector::ConnectorPtr connector(new Connector(&loop, addr));
    connector->setNewConnectionCallback(connectionCallback);
    connector->start();

    loop.loop();
}

