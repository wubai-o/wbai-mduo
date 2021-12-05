#include"../study/eventloop.h"

study::EventLoop* g_loop;
study::TimerId toCancel;

void cancelSelf() {
    printf("cancelSelf");
    g_loop->cancel(toCancel);
}

int main() {
    study::EventLoop loop;
    g_loop = &loop;
    toCancel = loop.runEvery(5, cancelSelf);
    loop.loop();
}