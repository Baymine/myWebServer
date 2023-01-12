#include "EventLoop.h"
#include "Epoll.h"
#include "Channel.h"
#include <vector>

EventLoop::EventLoop() : ep(nullptr), quit(false){
    ep = new Epoll();
}

EventLoop::~EventLoop(){
    delete ep;
}

// 处理队列中的事件
void EventLoop::loop(){
    while(!quit){
        std::vector<Channel*> chs;
        chs = ep->poll(); // 返回所有触发的channel
        for (auto it = chs.begin(); it != chs.end(); it++){
            (*it)->handleEvent();  // 处理每一个channel
        }
    }
}


void EventLoop::updateChannel(Channel *ch){
    ep->updateChannel(ch);
}