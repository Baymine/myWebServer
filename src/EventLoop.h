#pragma once
class Epoll;
class Channel;

/*
 * 处理触发事件的类
 * 1. 利用回调函数
 * */
class EventLoop
{
private:
    Epoll *ep;
    bool quit;
public:
    EventLoop();
    ~EventLoop();

    void loop();  // 处理队列中的事件(事件驱动)
    void updateChannel(Channel*);  // 更新监听epoll的监听事件
};