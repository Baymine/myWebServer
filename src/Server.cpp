#include "Server.h"
#include "Socket.h"
#include "InetAddress.h"
#include "Channel.h"
#include <functional>
#include <cstring>
#include <unistd.h>
#include "Acceptor.h"
#include "Connection.h"

#include <cstdio>
#include <cerrno>

#define READ_BUFFER 1024

// Server::Server(EventLoop *_loop) : loop(_loop){
//     Socket *serv_sock = new Socket();
//     InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
//     serv_sock->bind(serv_addr);
//     serv_sock->listen();
//     serv_sock->setnonblocking();

//     Channel *servChannel = new Channel(loop, serv_sock->getFd());
//     std::function<void()>cb = std::bind(&Server::newConnection, this, serv_sock);
//     servChannel->setCallback(cb);
//     servChannel->enableReading();
// }

Server::Server(EventLoop *_loop):loop(_loop), acceptor(nullptr){
    acceptor = new Acceptor(loop);
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);  // 设置创建连接的回调函数
}

Server::~Server() { 
    delete acceptor;
}

void Server::handleReadEvent(int sockfd){
    char buf[READ_BUFFER];

    while(true){
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        //非阻塞IO，这个条件表示数据全部读取完毕
        } else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
            printf("finish reading once, error:%d\n", errno);
            break;
        // EOF， 客户端断开连接
        } else if(bytes_read == 0){
            printf("EOF, client fd %d disconnected\n", sockfd);
            break;
        }
    }
}

//void Server::newConnection(Socket *serv_sock){
//    InetAddress *clnt_addr = new InetAddress();
//    Socket *clnt_sock = new Socket(serv_sock->accept(clnt_addr));
//
//    printf("new client fd %d! IP: %s Port: %d\n",
//            clnt_sock->getFd(), inet_ntoa(clnt_addr->addr.sin_addr), ntohs(clnt_addr->addr.sin_port));
//    clnt_sock->setnonblocking();
//
//    Channel *clnChannel = new Channel(loop, clnt_sock->getFd());
//    std::function<void()> cb = std::bind(&Server::handleReadEvent, this, clnt_sock->getFd());
//    clnChannel->setCallback(cb);
//    clnChannel->enableReading();
//}

// 新建连接放到一个map中
void Server::newConnection(Socket *sock) {
    Connection *conn = new Connection(loop, sock);
    std::function<void(Socket*)>cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = conn;
}

// 从map中删除，释放地址
void Server::deleteConnection(Socket *sock) {
    Connection *conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}