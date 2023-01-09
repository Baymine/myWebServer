#include "Channel.h"
#include "EventLoop.h"

Channel::Channel(EventLoop *_loop, int _fd) :
        loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false){  }

Channel::~Channel() {   }

void Channel::handleEvent(){
    callback();
}

// 设置监听事件以及模式，并更新channel
void Channel::enableReading(){
    // EPOLLIN:连接到达；有数据来临；
    // EPOLLET: ET模式
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}

uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(){
    inEpoll = true;
}

void Channel::setRevents(uint32_t _ev){
    revents = _ev;
}

void Channel::setCallback(std::function<void()> _cb){
    callback = _cb;
}