#pragma once
#include<vector>
#include<map>
#include "Socket.h"


class Connection{
private:
    //连接的client_sock：只负责释放
    std::vector<Socket*> socks; //不建议在vector添加指针，只在释放socket时使用
    
    //保存临时数据：在线数据的管理
    //std::vector<Usr> usr_vec;
public:
    Connection();

    //加入socket
    void add_Socket_ptr(Socket* _sock);
    //删除socket
    void free_Socket_ptr();       
};