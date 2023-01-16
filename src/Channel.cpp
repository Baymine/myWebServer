#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *_loop, int _fd) :
        loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false), useThreadPool(true){  }

Channel::~Channel() {   }

void Channel::handleEvent(){
    if(ready & (EPOLLIN | EPOLLPRI)){
        if(useThreadPool)
            loop->addThread(readCallback);
        else
            readCallback();
    }
    if(ready & (EPOLLOUT)){
        if(useThreadPool)
            loop->addThread(writeCallback);
        else
            writeCallback();
    }
}

// 设置监听事件以及模式，并更新channel
void Channel::enableRead(){
    // EPOLLIN:连接到达；有数据来临；
    // EPOLLET: ET模式
    events |= EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}

uint32_t Channel::getReady() {
    return ready;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(){
    inEpoll = true;
}

void Channel::setReady(uint32_t _ev) {
    ready = _ev;
}

void Channel::setReadCallback(std::function<void()> _cb) {
    readCallback = _cb;
}

void Channel::setUseThreadPool(bool use) {
    useThreadPool = use;
}