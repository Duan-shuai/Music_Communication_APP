#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>

#define BUFSIZE 100

int main(){
    //创建socket
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;//inet_addr("192.168.92.128"); //ip
    serv_addr.sin_port = htons(8888); //端口

    //bind
    bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    //listen
    listen(serv_sock, 32);

    //accept
    int client_sock = accept(serv_sock, NULL, NULL);
    if(client_sock < 0)
        std::cout<<"connection error"<<std::endl;
    while(1){
        //recv
        char strbuffer[BUFSIZE]{0};
        ssize_t data = recv(client_sock, strbuffer, BUFSIZE, 0);
        std::cout<<"recv:"<<strbuffer<<std::endl;

        if('q' == strbuffer[0] && strbuffer[1] == '\0'){
            send(client_sock, "bye", BUFSIZE, 0);
            break;
        }

        //send
        int sendlen = send(client_sock, "ok", BUFSIZE, 0);
        if(sendlen < 0)
            std::cout<<"send error"<<std::endl;
    }

    close(serv_sock);
    close(client_sock);

    system("pause");
    return 0;
}