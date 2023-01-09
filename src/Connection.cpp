//
// Created by root on 2023/1/6.
//
#include "Connection.h"
#include "Socket.h"
#include "Channel.h"
#include <unistd.h>
#include <cstring>
#include <cstdio>
#include <cerrno>
#include <utility>

#define READ_BUFFER 1024

Connection::Connection(EventLoop *_loop, Socket *_sock):loop(_loop), sock(_sock), channel(nullptr) {
    channel = new Channel(loop, sock->getFd());
    std::function<void()>cb = [this, capture0 = sock->getFd()] { echo(capture0); };
    channel->setCallback(cb);
    channel->enableReading();
}

Connection::~Connection() {
    delete channel;
    delete sock;
}

// 响应读事件
void Connection::echo(int sockfd) {
    char buf[READ_BUFFER];
    while (true){
        bzero(&buf, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if (bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));
        } else if(bytes_read == -1 && errno == EINTR){
            printf("Continue reading");
            continue;
        } else if(bytes_read == -1 &&((errno == EAGAIN) || (errno == EWOULDBLOCK))){
            printf("finish reading once, errno: %d\n", sockfd);
        } else if(bytes_read == 0){
            printf("EOF, client fd %d disconnect\n", sockfd);
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket *)> _cb) {
    deleteConnectionCallback = std::move(_cb);
}