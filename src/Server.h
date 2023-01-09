#pragma once
#include <map>

class EventLoop;
class Socket;
class Acceptor;
class Connection;
class Server{
private:
    EventLoop *loop;  // 事件循环，事件驱动类
    Acceptor *acceptor;
    std::map<int, Connection*> connections;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket *serv_sock);
    void deleteConnection(Socket *sock);
};