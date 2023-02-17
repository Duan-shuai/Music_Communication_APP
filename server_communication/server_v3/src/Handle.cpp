#include "Handle.h"
#include "util.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<iostream>
#include<errno.h>

Handle::Handle(){
    db = Database::GetInstance();
}

Json::Value Handle::string2Json(std::string laser){
    Json::Value tree;
    Json::String err;
    Json::CharReaderBuilder reader;
    std::unique_ptr<Json::CharReader>const json_read(reader.newCharReader());
    json_read->parse(laser.c_str(), laser.c_str() + laser.length(), &tree, &err);
    return tree;
}

std::string Handle::Json2string(Json::Value root){
    Json::StreamWriterBuilder writerBuilder;
    std::unique_ptr<Json::StreamWriter> json_write(writerBuilder.newStreamWriter());
    std::ostringstream ss;
    json_write->write(root, &ss);
    std::string strContent = ss.str();
    return strContent;
}


void Handle::parse_message(int socket){

    //接受消息并转化：char[]->string->json
    char recvbuf[RECV_BUFFER]{0};
    ssize_t data = recv(socket, recvbuf, RECV_BUFFER, 0);
    errif(data == -1 && errno != EAGAIN, "recv error");   
    std::string recv_str = recvbuf;
    std::cout<<recv_str<<std::endl;
    Json::Value root = string2Json(recv_str);

    Json::Value msg;

    if(root["HEAD"] == "LOGIN"){
        int id = std::stoi(root["ID"]);
        passwd = root["PASSWD"];
        std::string res = db->log_validation(id, passwd);
        if(res.size() == 0){
            msg["HEAD"] = "LOGIN";
            msg["BODY"] = "0";
        }
        else{
            msg["HEAD"] = "LOGIN";
            msg["BODY"] = res;
        }
        std::string send_str = Json2string(msg);
        std::cout<<senf_str<<std::endl;
    }

    else if(root["HEAD"] == "REGIST"){

    }

    else if(root["HEAD"] == "SEND"){
        std::cout<<"get a message"<<std::endl;
    }
}

void Handle::sendMsg(int socket){
    char sendbuf[SEND_BUFFER]{0};
    //todo:完成写逻辑
    int sendLen = send(socket, sendbuf, SEND_BUFFER, 0);   
}

/*
todo:
1.关闭client,防止内存泄漏
注意：
1.recv不能阻塞，否则无法break
*/
std::string Handle::recvMsg(int socket){
    char recvbuf[RECV_BUFFER]{0};
    std::string recv_str;
    while(1){
        ssize_t data = recv(socket, recvbuf, RECV_BUFFER, 0);
        errif(data == -1 && errno != EAGAIN, "recv error");       
        //数据已经读完
        if(data == -1 && errno == EAGAIN){
            printf("\n");
            break;
        }
        //断开连接 
        else if(data == 0){
            std::cout<<"client leaves out"<<std::endl;
            //todo:close client_sock
            break;
        }
        //循环读取缓存区
        else{
            write(STDOUT_FILENO, recvbuf, RECV_BUFFER);
            //std::cout<<"recv:"<<recvbuf<<std::endl; //会出现乱码
        }
    }
    recv_str = recvbuf;
    return recv_str;
}

//发送文件 
void Handle::sendFile(char file_name[], int socket){
	// 打开文件并读取文件数据
   	FILE *fp = fopen(file_name, "r");
    errif(fp == NULL, "file doesn't exist");

    char sendbuf[SEND_BUFFER]{0};
    int length = 0;
    int sendlen = 0;
    while(1){
        //fp -> sendbuf
        length = fread(sendbuf, sizeof(char), SEND_BUFFER, fp);
        errif(length == -1, "read error");
        if(length == 0) break; 
        //sendbuf -> socket
        sendlen = send(socket, sendbuf, length, 0);
        errif(sendlen == -1, "send error"); 
        //清空缓存
        bzero(sendbuf, SEND_BUFFER);
    }
    fclose(fp);
}

void Handle::recvFile(char file_name[], int socket){
    //创建并准备写入文件
    FILE *fp = fopen(file_name, "w");
    errif(fp == NULL, "file can't create");

    char recvbuf[RECV_BUFFER]{0};
    int length = 0;
    int recvlen = 0;
    while(1){
        //socket -> recvbuf
        recvlen = recv(socket, recvbuf, RECV_BUFFER, 0);
        errif(recvlen == -1, "recv error"); 
        //recvbuf -> fp
        length = fwrite(recvbuf, sizeof(char), length, fp);
        errif(length == -1, "write error");
        if(length == 0) break; 
        //清空缓存
        bzero(recvbuf, RECV_BUFFER);
    }
}