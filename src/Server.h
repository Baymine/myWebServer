#pragma once
#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;

// 将服务器类与事件驱动类进行了分离

class Server{
private:
    EventLoop *loop;  // 事件驱动类，一个事件循环，我们添加需要监听的事务到这个事件循环内
    Acceptor *acceptor;
    std::map<int, Connection*> connections; // fd : connection
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
};