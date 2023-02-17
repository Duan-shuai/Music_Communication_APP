#include "handle_json.h"
#include<QDebug>

Handle_Json::Handle_Json(QObject *parent) : QObject(parent){}

QString Handle_Json::QJson2QString(QJsonObject jsonObject){
    return QString(QJsonDocument(jsonObject).toJson());
}

QJsonObject Handle_Json::QString2QJson(QString jsonString){
    QJsonDocument jsonDocument = QJsonDocument::fromJson(jsonString.toLocal8Bit().data());
        if(jsonDocument.isNull())
        {
            qDebug()<< "String NULL"<< jsonString.toLocal8Bit().data();
        }
        QJsonObject jsonObject = jsonDocument.object();
        return jsonObject;
}
/*
 * 协议设计(SERVER REPLY)
 * 登录：
 * HEAD="LOGIN"
 * BODY="1" OR "0"
 * 注册：
 * HEAD="REGIST"
 * BODY="ID" OR "0"
 */
QString Handle_Json::parse_msg(QJsonObject json_msg){
    if(json_msg["HEAD"] == "LOGIN"){
        QString s = json_msg.value("BODY").toString();
        return s;
    }
    
    if(json_msg["HEAD"] == "REGIST"){
        QString s = json_msg.value("BODY").toString();
        return s;
    }
    return NULL;
}
