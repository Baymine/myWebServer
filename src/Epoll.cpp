#include "Epoll.h"
#include "util.h"
#include <unistd.h>
#include <string.h>
#include "Channel.h"

#define MAX_EVENTS 1000

Epoll::Epoll() : epfd(-1), events(nullptr){
    epfd = epoll_create1(0);
    errif(-1 == epfd, "epoll create failed");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events) * MAX_EVENTS);
}

Epoll::~Epoll() {
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

void Epoll::addFd(int fd, uint32_t op){
    struct epoll_event event;
    bzero(&event, sizeof(event));
    event.data.fd = fd;
    event.events = op;
    // 为epfd添加一个监听事件，fd:操作的目标描述符， event：监听事件
    errif(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &event), "epoll add event failed");
}

// // Activated events list
// std::vector<epoll_event> Epoll::poll(int timeout){
//     std::vector<epoll_event> activeEvents;
//     int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
//     errif(-1 == nfds, "epoll wait failed");
//     for(int i = 0; i < nfds; i++){
//         activeEvents.push_back(events[i]);
//     }
//     return activeEvents;
// }


std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> activeChannels;
    // epfd:监听fd. events:监听事件列表
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errif(-1 == nfds, "epoll wait error");

    for(int i = 0; i < nfds; ++i){
        Channel *ch = (Channel*) events[i].data.ptr;  // 一个嵌入指针（void*）
        ch->setRevents(events[i].events);  // 返回该channel时正在发生的事件
        activeChannels.push_back(ch);
    }
    return activeChannels;
}

// 根据channel中的信息，调整epoll事件
void Epoll::updateChannel(Channel* channel) const{
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->getInEpoll()){
        errif(-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev), "epoll add error");
        channel->setInEpoll();
    } else {
        errif(-1 == epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev), "epoll modify error");
    }
}