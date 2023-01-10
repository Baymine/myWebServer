//
// Created by root on 2023/1/9.
//
#pragma once
#include <string>

class Buffer{
private:
    std::string buf;

public:
    Buffer();
    ~Buffer();

    void append(const char *_str, int _size);
    ssize_t size();
    const char *c_str();
    void clear();
    void getline();
};