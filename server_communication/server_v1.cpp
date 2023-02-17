#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

/*
单向通信成功
*/
int main(){

    //创建socket
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; //ip
    serv_addr.sin_port = htons(8888); //端口

    //bind
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //listen
    listen(serv_sock, 32);

    //accept
    //struct sockaddr_in clnt_addr;
    //socklen_t clnt_addr_size = sizeof(clnt_addr);
    int client_sock = accept(serv_sock, NULL, NULL);

    //send
    send(client_sock, "ok", sizeof("ok"), 0);

    close(serv_sock);
    close(client_sock);
}