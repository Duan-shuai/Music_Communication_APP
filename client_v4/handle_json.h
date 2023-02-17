#ifndef HANDLE_JSON_H
#define HANDLE_JSON_H

#include <QObject>
#include<QJsonObject>
#include<QJsonDocument>

class Handle_Json : public QObject
{
    Q_OBJECT
public:
    explicit Handle_Json(QObject *parent = nullptr);

signals:

public:
    QJsonObject QString2QJson(QString);
    QString QJson2QString(QJsonObject);
    QString parse_msg(QJsonObject json_msg);
};

#endif // HANDLE_JSON_H
