#include"../study/eventloop.h"
#include"../study/EventLoopThread.h"
#include<stdio.h>

void runInThread() {
    printf("runInThread(): pid = %d, tid = %d\n", getpid(), study::CurrentThread::tid());
}

int main() {
    printf("main(): pid = %d, tid = %d", getpid(), study::CurrentThread::tid());
    study::EventLoopThread loopThread;
    study::EventLoop* loop = loopThread.startLoop();
    loop->runInLoop(runInThread);
    sleep(2);
    loop->quit();

    printf("exit main().\n");
}