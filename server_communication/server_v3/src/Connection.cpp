#include<iostream>

#include "Connection.h"


Connection::Connection(){}

void Connection::add_Socket_ptr(Socket* _sock){
    socks.push_back(_sock);
}

//析构，防止内存泄漏
void Connection::free_Socket_ptr(){
    for(auto sock : socks){
        delete sock;
    }
}