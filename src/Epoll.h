#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;

/*
 * 管理epoll事件的类
 * 1. 添加或改变epoll事件
 * 2. 返回触发的事件
 * */
class Epoll
{
private:
    int epfd;
    struct epoll_event *events;

public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op); // // 为epoll添加一个监听事件
    // std::vector<epoll_event> poll(int timeout = -1);
    std::vector<Channel*> poll(int timeout = -1); // 将所有激发的事件存储到一个channel向量中，并返回这个向量
    void updateChannel(Channel*) const;  // 根据channel中的信息，调整epoll事件
};