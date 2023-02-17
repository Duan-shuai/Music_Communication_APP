#pragma once
#include <sys/epoll.h>
#include <vector>

class Epoll
{
private:
    int epfd; //epoll描述符
    struct epoll_event *events;  //epoll上的事件
public:
    Epoll();
    ~Epoll();
    //添加事件文件描述符
    void addFd(int fd, int event_op);
    //删除事件文件描述符
    void delFd(int fd);
    /*
    等待
    return：活跃事件数目
    */
    int wait();
    /*
    获取活跃事件的vector,注意每次调用epoll_wait时更新events
    return：活跃事件的vector
    */
    std::vector<epoll_event> get_events(int timeout = -1);
};
/*
LT：
如果调用wait后没有一次处理完事件，下次依然会返回事件
ET：
如果调用wait后没有一次处理完事件，不会再次返回事件

Q：若读缓存区一次不能读完，出现什么情况？
LT：多次调用wait，分批次读完。
ET：下次发送事件触发时，继续传送。

Q：ET解决方案？
1.读缓存大于写缓存
2.while循环读取

Q：ET+非阻塞？
ET+非阻塞+while，否则会阻塞在recv中无法继续
*/
