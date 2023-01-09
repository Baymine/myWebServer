#include "Socket.h"
#include "InetAddress.h"
#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

Socket::Socket() : fd(-1){
    fd = socket(AF_INET, SOCK_STREAM, 0);
    errif(-1 == fd, "socket create error");
}

Socket::Socket(int _fd) : fd(_fd){
    errif(-1 == fd, "socket create error"); 
}

Socket ::~Socket() {
    if(fd != -1){  // close file descriptor and reset
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress* addr){
    errif(-1 == ::bind(fd, (sockaddr*)&addr->addr, addr->addr_len), 
                        "socket bind error");
}

void Socket::listen(){
    errif(
        -1 == ::listen(fd, SOMAXCONN),
        "socket listen error"
    );
}

void Socket::setnonblocking() {
    fcntl(fd, F_SETFL, fcntl(fd, F_SETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress* address) {
    int client_socket = ::accept(fd, (sockaddr*)&address->addr, &address->addr_len);
    errif(client_socket == -1, "socket accept failed");
    return client_socket;
}

int Socket::getFd(){
    return fd;
}
