#ifndef SIGNWINDOW_H
#define SIGNWINDOW_H

#include <QMainWindow>
#include <QPoint>
#include <QMouseEvent>
#include "musicwindow.h"
#include "client_socket.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SignWindow; }
QT_END_NAMESPACE

class SignWindow : public QMainWindow
{
    Q_OBJECT

public:
    SignWindow(QWidget *parent = nullptr);
    ~SignWindow();

private:
    Ui::SignWindow *ui;
    MusicWindow *musicwindow;

    Client_Socket* client_sock;

    //页面移动相关
    bool isPressedWidget;
    QPoint last;
    void mousePressEvent(QMouseEvent *event); // 鼠标点击
    void mouseMoveEvent(QMouseEvent *event); // 鼠标移动
    void mouseReleaseEvent(QMouseEvent *event); // 鼠标释放


public slots:
    void slot_sign_in();
    void slot_sign_up();
    //重写函数
    void slot_Min_clicked();
    void slot_Close_clicked();

    void slot_Newpage_clicked();
};
#endif // SIGNWINDOW_H
