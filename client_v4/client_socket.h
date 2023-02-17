#ifndef CLIENT_SOCKET_H
#define CLIENT_SOCKET_H

#include <QTcpSocket>
/*
 单例模式:静态局部方式实现，线程安全
 */
class Client_Socket
{
private:
    bool isconnetion; //判断是否连接到服务器的标志位
    QTcpSocket sock; //发送消息套接字
private:
    Client_Socket(){
        isconnetion = false;
        //sock已经默认初始化

    }
    ~Client_Socket(){}
    //避免重复赋值
    Client_Socket(const Client_Socket&);
    Client_Socket& operator=(const Client_Socket&);

public:
    static Client_Socket* GetInstance(){
        static Client_Socket client_sock;
        return &client_sock;
    }

    bool Isconnection();
    void Setconnection(bool in);
    void Abort();

    void ConnectTo(QString ip, quint16 port);
    int SendString(const QString&);
    QByteArray RecvString();
};

#endif // CLIENT_SOCKET_H


