#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include<iostream>
#include "Epoll.h"
#include "Socket.h"
#include "Handle.h"
#include "Connection.h"

#define MAX_EVENTS 1024

int main() {
    //每个线程一个ep、h、connection
    Epoll *ep = new Epoll();
    Handle h;
    Connection connection;

    //Listen_socket
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("192.168.92.128", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonblocking();   
    
    //将listen_fd加入epoll的事件红黑树
    ep->addFd(serv_sock->getFd(), EPOLLIN);

    while(true){
        std::vector<epoll_event> events = ep->get_events();
        for(auto event : events){           
            //connection
            if(event.data.fd == serv_sock->getFd())
            {
                int cfd = serv_sock->accept();
                Socket *s = new Socket(cfd);
                s->setnonblocking();
                ep->addFd(cfd, EPOLLIN);

                //加入connection防止内存泄漏
                connection.add_Socket_ptr(s);
            }
            //communication,逻辑由handle完成
            else{
                if(event.events & EPOLLIN)
                {
                    /*
                    协议设计：Json类型
                    HEAD + BODY
                    */
                    h.parse_message(event.data.fd);
                }
                // else if(event.events & EPOLLOUT)
                // {
                //     h.sendMsg(event.data.fd);
                // }
            }
        }
    }
    delete serv_sock;
    delete serv_addr;
    return 0;
}
