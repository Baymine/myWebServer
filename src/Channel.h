#pragma once
#include <sys/epoll.h>
#include <functional>
class EventLoop;

class Channel{
private:
    EventLoop *loop;
    int fd;
    uint32_t events;  // 希望监听这个文件描述符的哪些事件，因为不同事件的处理方式不一样
    uint32_t revents; // 在epoll返回该Channel时文件描述符正在发生的事件
    bool inEpoll;  // 当前Channel是否已经在epoll红黑树中
    std::function<void()> callback; // void: return type; 括号内: parameter types

public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();
    void enableReading();

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll();

    void setRevents(uint32_t);
    void setCallback(std::function<void()>);
};