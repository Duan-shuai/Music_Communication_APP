#pragma once
#include<string>
#include "../jsoncpp/jsoncpp.cpp"

/*
客户端页面设置：
1.登陆注册页面：SIGN
2.个人页面：GET
3.new_friend页面：NEW_FRIEND
4.聊天页面：COMMUNICATION
*/
/*
应用层协议：
1.SIGN:sign up,sign in
2.GET:new_friend,communication
3.COMMUNICATION:text,file
*/
class ProtocolMsg{
private:
    int head;
    Json::Value body;
public:
    ProtocolMsg(int _head, Json::Value _body);
    ~ProtocolMsg();
};
