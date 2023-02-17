#ifndef HANDLE_H
#define HANDLE_H

#define SEND_BUFFER 1024
#define RECV_BUFFER 1024
#include<string>
#include "json/json.h"
#include "Database.h"

/*
parse_message()
HEAD类型:   
1.regist： 
查询持久性存储，判别id合法性； 
信息加入持久性存储； 
2.login：
创建Usr； 
将Usr加入到临时存储； 
推送相关内容；
*/
class Handle{
private:
    Database* db;
public:
    Handle();
    //接受客户端的信号，处理不同任务
    void parse_message(int socket);

    //辅助函数：json与string转换
    std::string Json2string(Json::Value root);
    Json::Value string2Json(std::string str);

    //给特定的socket发送文本、文件
    void sendMsg(int socket);
    void sendFile(char file_name[], int socket);

    //给特定的socket发送文本、文件
    std::string recvMsg(int socket);
    void recvFile(char file_name[], int socket);
};

# endif