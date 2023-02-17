#include <QMessageBox>
#include<QJsonObject>

#include "handle_json.h"
#include "signwindow.h"
#include "ui_signwindow.h"
//#include "communicationwindow.h"

SignWindow::SignWindow(QWidget *parent):QMainWindow(parent), ui(new Ui::SignWindow)
{
    ui->setupUi(this);

    //删除头，后续重写最大化、最小化、关闭
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    //设置背景
    QImage *img=new QImage;;
    img->load("D:\\duan2.png");
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(*img));
    //连接sock
    client_sock = Client_Socket::GetInstance();
    client_sock->Abort();
    client_sock->ConnectTo("192.168.92.128",8888);
    client_sock->Setconnection(true);

    connect(ui->pushButton_in, SIGNAL(clicked()), this, SLOT(slot_sign_in()));
    connect(ui->pushButton_up, SIGNAL(clicked()), this, SLOT(slot_sign_up()));
    connect(ui->pushButton_min, SIGNAL(clicked()), this, SLOT(slot_Min_clicked()));
    connect(ui->pushButton_close, SIGNAL(clicked()), this, SLOT(slot_Close_clicked()));

    connect(ui->pushButton_vsist, SIGNAL(clicked()), this, SLOT(slot_Newpage_clicked()));
}

void SignWindow::slot_sign_in(){

    QJsonObject obj;
    Handle_Json hj;

    //构造报文
    QString username = ui->id_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    obj.insert("HEAD","LOGIN");
    obj.insert("ID",username);
    obj.insert("PASSWD",password);
    QString send_str = hj.QJson2QString(obj);
    //发送
    client_sock->SendString(send_str);
    //接收
    QString recv_msg = client_sock->RecvString();
    QJsonObject recv_json = hj.QString2QJson(recv_msg);
    QString flag = hj.parse_msg(recv_json);

    if(flag == "1"){//登录成功
        //personalwindow = new CommunicationWindow(this);
        //personalwindow->show();
        this->hide();
    }
    else{//登录失败
        QMessageBox::about(this, "failed", "sign in again!");
    }
}

void SignWindow::slot_sign_up(){
    QJsonObject obj;
    Handle_Json hj;

    //构造报文
    QString username = ui->id_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    obj.insert("HEAD","REGIST");
    obj.insert("USER", username);
    obj.insert("PASSWD", password);
    QString send_str = hj.QJson2QString(obj);
    //发送
    client_sock->SendString(send_str);
    //接收
    QString recv_msg = client_sock->RecvString();
    QJsonObject recv_json = hj.QString2QJson(recv_msg);
    QString flag = hj.parse_msg(recv_json);

    if(flag == "0"){//注册失败
        QMessageBox::about(this, "failed", "sign up again!");
    }
    else{//注册成功
        QMessageBox::about(this, "successful", "your id is " + flag);
    }
}

void SignWindow::slot_Newpage_clicked(){
    musicwindow = new MusicWindow();
    musicwindow->show();
    this->hide();
}

SignWindow::~SignWindow()
{
    delete ui;
}

//~~~~~~~~~~~~~~ 界面移动、最大化、最小化、关闭 ~~~~~~~~~~~~~~~~~~~
void SignWindow::mousePressEvent(QMouseEvent *event) {
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void SignWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isPressedWidget) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void SignWindow::mouseReleaseEvent(QMouseEvent *event) {
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void SignWindow::slot_Min_clicked(){
    showMinimized();
}

void SignWindow::slot_Close_clicked(){
    close();
}
//~~~~~~~~~~~~~~~ 界面移动、最大化、最小化、关闭结束 ~~~~~~~~~~~~~~~~

