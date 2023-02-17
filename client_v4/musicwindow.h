#ifndef MUSICWINDOW_H
#define MUSICWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTime>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFont>
#include <QTextStream>
#include <QString>
#include <QListWidgetItem>
#include <QColor>
#include <QPoint>
#include <QDir>

namespace Ui {
class MusicWindow;
}

class MusicWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MusicWindow(QWidget *parent = nullptr);
    ~MusicWindow();

public slots:
    //重写函数
    void slot_Min_clicked();
    void slot_Close_clicked();
    //按钮
    void slot_start_clicked();
    void slot_premusic_clicked();
    void slot_nextmusic_clicked();
    //拖拽
    void on_slider_volume_valueChanged(int value);
    void slot_voice_clicked();
    void slot_DurationChanged(qint64 duration);
    void slot_PositionChanged(qint64 position);

private:
    Ui::MusicWindow *ui;
    void init();

    //辅助函数
    QStringList getFileNames(const QString &path);
    void switchPage();
    //页面移动相关
    bool isPressedWidget;
    QPoint last;
    void mousePressEvent(QMouseEvent *event); // 鼠标点击
    void mouseMoveEvent(QMouseEvent *event); // 鼠标移动
    void mouseReleaseEvent(QMouseEvent *event); // 鼠标释放

    //播放相关
    bool isPlay = false;
    QString MusicPath;  //文件地址
    QStringList fileList;  //文件名称
    QMediaPlaylist *PlayerList;  //播放列表
    QMediaPlayer *Player;  //当前播放
    QString totalTime;    // 音乐总时长
    QString currTime;     // 当前播放时长

    //拖拽
    int musicVolume_last;
    int musicVolume_now;
    bool voiceOpenFlag;

public:
    void insertmusiclist(int rowNum, QString musicName, QString singer, QString size);
    void initMyMusic();
};

#endif // MUSICWINDOW_H
