#pragma once
#include <functional>

class EventLoop;
class Socket;
class InetAddress;
class Channel;
class Acceptor{
private:
    EventLoop *loop;
    Socket *sock;  // 包含所有连接的信息
    InetAddress *addr;
    Channel *acceptChannel;

public:
    Acceptor(EventLoop *_loop);
    ~Acceptor();
    void acceptConnection();
    std::function<void(Socket*)> newConnectionCallback;
    void setNewConnectionCallback(std::function<void(Socket*)>);
};