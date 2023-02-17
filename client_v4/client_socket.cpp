#include "client_socket.h"
#include <QHostAddress>

bool Client_Socket::Isconnection(){
    return isconnetion;
}

void Client_Socket::Setconnection(bool in){
    isconnetion = in;
}

void Client_Socket::Abort(){
    sock.abort();
}

void Client_Socket::ConnectTo(QString ip, quint16 port){
    sock.connectToHost(QHostAddress(ip), port);
}

int Client_Socket::SendString(const QString& message){
    if(!message.isEmpty())
        sock.write(message.toUtf8());
     return message.size();
}

QByteArray Client_Socket::RecvString(){
    return sock.readAll();
}
