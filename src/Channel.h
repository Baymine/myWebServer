#pragma once
#include <sys/epoll.h>
#include <functional>
class EventLoop;
class Socket;

/**
 * epoll中的data其实是一个union类型，可以储存一个指针。而通过指针，理论上我们可以指向任何一个地址块的内容，可以是一个类的对象，
 *
 * 这样就可以将一个文件描述符封装成一个Channel类，一个Channel类自始至终只负责一个文件描述符，对不同的服务、不同的事件类型，
 * 都可以在类中进行不同的处理，而不是仅仅拿到一个int类型的文件描述符。
 */
class Channel{
private:
    EventLoop *loop;
    int fd;
    uint32_t events;  // 希望监听这个文件描述符的哪些事件，因为不同事件的处理方式不一样
    uint32_t ready; // 在epoll返回该Channel时文件描述符正在发生的事件
    bool inEpoll;  // 当前Channel是否已经在epoll红黑树中
    bool useThreadPool;
    std::function<void()> readCallback; // void: return type; 括号内: parameter types
    std::function<void()> writeCallback;


public:
    Channel(EventLoop *_loop, int _fd);
    ~Channel();

    void handleEvent();  // 处理事件，调用回调函数
    void enableRead();  //  设置监听事件以及模式，并更新channel

    int getFd();
    uint32_t getEvents();
    uint32_t getReady();
    bool getInEpoll();  // inEpoll
    void setInEpoll();  // inEpoll = true;
    void useET();

    void setReady(uint32_t);
    void setReadCallback(std::function<void()>);
    void setUseThreadPool(bool use = true);
};