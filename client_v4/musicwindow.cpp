#include "musicwindow.h"
#include "ui_musicwindow.h"

MusicWindow::MusicWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MusicWindow)
{
    ui->setupUi(this);
    //删除头，后续重写最大化、最小化、关闭
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    connect(ui->pushButton_min, SIGNAL(clicked()), this, SLOT(slot_Min_clicked()));
    connect(ui->pushButton_close, SIGNAL(clicked()), this, SLOT(slot_Close_clicked()));
    //三种界面切换
    connect(ui->btn_main_page, &QToolButton::clicked, this, &MusicWindow::switchPage);
    connect(ui->btn_music, &QToolButton::clicked, this, &MusicWindow::switchPage);
    connect(ui->btn_message, &QToolButton::clicked, this, &MusicWindow::switchPage);
    //播放，下一首，前一首
    connect(ui->play, &QToolButton::clicked, this, &MusicWindow::slot_start_clicked);
    connect(ui->pre_music, &QToolButton::clicked, this, &MusicWindow::slot_premusic_clicked);
    connect(ui->next_music, &QToolButton::clicked, this, &MusicWindow::slot_nextmusic_clicked);
    //音量相关
    connect(ui->slider_volume, &QSlider::valueChanged, this, &MusicWindow::on_slider_volume_valueChanged);
    connect(ui->btn_voice, &QToolButton::clicked, this, &MusicWindow::slot_voice_clicked);
    //初始化
    initMyMusic();
    init();
}

MusicWindow::~MusicWindow()
{
    delete ui;
}

void MusicWindow::init(){
    //实例化播放器和播放列表
    PlayerList = new QMediaPlaylist;
    Player = new QMediaPlayer;
    //声音
    voiceOpenFlag = true;
    musicVolume_now = musicVolume_last = 30;  //音量为30
    Player->setVolume(30);
    ui->slider_volume->setValue(30);

    //导入文件
    MusicPath = "D:\\work_item\\Qt\\client_v4\\music";
    fileList = getFileNames(MusicPath);
    //qDebug()<<"ALL Music File: \n "<<fileList;
    ui->tableWidget->setRowCount(fileList.size());

    for(int i=0;i<fileList.size();i++)
    {
        QString fileName = fileList.at(i);
        //qDebug()<<fileName;
        //addItem(fileName); //输出所有音频文件到界面
        PlayerList->addMedia(QUrl::fromLocalFile(MusicPath+"\\"+fileName)); //音乐加载到playlist

        //将音乐加入到weight中
        QFileInfo info(MusicPath+"\\"+fileName);
        QStringList strlist = info.fileName().split("-");    // 分割后：[0]歌名   [1]歌手.mp3
        QString musicName = strlist[0].trimmed();            // 歌名
        int index = strlist[1].lastIndexOf(".");             // 歌手(index).mp3
        QString singer = strlist[1].mid(0,index);            // 歌手
        QString size = QString::number(info.size()/1024/1024.00,'f',1)+"M";// 音乐大小
        //插入tablewidget中
        insertmusiclist(i, musicName, singer, size);
        qDebug()<<musicName<<","<<singer<<","<<size<<"\n";
    }

    PlayerList->setCurrentIndex(0);
    PlayerList->setPlaybackMode(QMediaPlaylist::Loop);//循环播放
    Player->setPlaylist(PlayerList);//将播放列表加载到播放器
    // 当媒体文件变化时发射durationChanged信号：用于获取媒体总时长
    connect(Player,&QMediaPlayer::durationChanged,this,&MusicWindow::slot_DurationChanged);
    // 当前媒体文件进度发生变化时发射此信号：用于实时获取播放进度
    connect(Player,&QMediaPlayer::positionChanged,this,&MusicWindow::slot_PositionChanged);
    //connect(Player,&QMediaPlayer::currentMediaChanged,this,&MusicWindow::SlotShowCurrentMedia);
}
// 我的音乐页面
void MusicWindow::initMyMusic()
{
    // 隐藏网格线
    ui->tableWidget->setShowGrid(false);
    // 去除纵向表头
    ui->tableWidget->verticalHeader()->hide();
    // 去除边框
    ui->tableWidget->setFrameShape(QFrame::NoFrame);
    // 不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 隐藏滚动条
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 取消焦点
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    // 设置行选择
    ui->tableWidget->setSelectionBehavior(QTableWidget::SelectRows);
    // 单元格延伸后实现单元格平分
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // 设置水平表头文字垂直居左
    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    // 设置垂直表头文字垂直居右
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignRight | Qt::AlignVCenter);
    // 设置水平表头高度
    ui->tableWidget->horizontalHeader()->setFixedHeight(50);
    // 设置行高，与表头高度无关
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(45);
    // 设置背景色交替
    ui->tableWidget->setAlternatingRowColors(true);
    // 设置点击表时不对水平表头进行高亮（获得焦点）
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    //  设置每行默认高度
    ui->tableWidget->verticalHeader()->setDefaultSectionSize(40);
}
//~~~~~~~~~~~~~~四个页面相关内容~~~~~~~~~~~~~
void MusicWindow::insertmusiclist(int rowNum, QString musicName, QString singer, QString size)
{
    QTableWidgetItem *item = new QTableWidgetItem(musicName);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip(musicName);
    ui->tableWidget->setItem(rowNum, 0, item);

    item = new QTableWidgetItem(singer);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    item->setToolTip(singer);
    ui->tableWidget->setItem(rowNum, 1, item);

    item = new QTableWidgetItem(size);
    item->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    ui->tableWidget->setItem(rowNum, 2, item);
}
//~~~~~~~~~~~~~~四个页面相关内容结束~~~~~~~~~~~~~

//~~~~~~~~~~~~~~辅助函数~~~~~~~~~~~~~
//找文件名
QStringList MusicWindow::getFileNames(const QString &path)
{

    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.mp3";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    return files;
}

void MusicWindow::switchPage(){
    QToolButton *button = qobject_cast<QToolButton*>(sender());//得到按下的按钮的指针
    if(button == ui->btn_main_page)
        ui->stackedWidget->setCurrentIndex(0);//根据按下的button按索引显示相应的页面
    else if(button==ui->btn_message)
        ui->stackedWidget->setCurrentIndex(1);
    else if(button==ui->btn_music)
        ui->stackedWidget->setCurrentIndex(2);

}
//~~~~~~~~~~~~~~辅助函数结束~~~~~~~~~~~~~

//~~~~~~~~~~~~~~ 界面移动、最大化、最小化、关闭 ~~~~~~~~~~~~~~~~~~~
void MusicWindow::mousePressEvent(QMouseEvent *event) {
    isPressedWidget = true; // 当前鼠标按下的即是QWidget而非界面上布局的其它控件
    last = event->globalPos();
}

void MusicWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isPressedWidget) {
        int dx = event->globalX() - last.x();
        int dy = event->globalY() - last.y();
        last = event->globalPos();
        move(x()+dx, y()+dy);
    }
}

void MusicWindow::mouseReleaseEvent(QMouseEvent *event) {
    int dx = event->globalX() - last.x();
    int dy = event->globalY() - last.y();
    move(x()+dx, y()+dy);
    isPressedWidget = false; // 鼠标松开时，置为false
}

void MusicWindow::slot_Min_clicked(){
    showMinimized();
}

void MusicWindow::slot_Close_clicked(){
    close();
}
//~~~~~~~~~~~~~~~ 界面移动、最大化、最小化、关闭结束 ~~~~~~~~~~~~~~~~

//~~~~~~~~~~~~~~~ 播放，前一首，后一首 ~~~~~~~~~~~~~~~~

void MusicWindow::slot_start_clicked(){
    if(!isPlay)
        {
            isPlay = true;
            ui->play->setIcon(QIcon(":/resource/bfzn_003.png"));
            Player->play();
        }
    else
        {
            isPlay = false;
            ui->play->setIcon(QIcon(":/resource/bfzn_004.png"));
            Player->pause();
        }
}

void MusicWindow::slot_premusic_clicked(){
    PlayerList->previous();
    PlayerList->previousIndex();
}

void MusicWindow::slot_nextmusic_clicked(){
    PlayerList->next();
    PlayerList->nextIndex();
}
//~~~~~~~~~~~~~~~ 结束 ~~~~~~~~~~~~~~~~
//~~~~~~~~~~~~~~~ 播放拖拽、声音拖拽 ~~~~~~~~~~~~~~~~
void MusicWindow::on_slider_volume_valueChanged(int value)
{
    //qDebug() << QString::number(ui->slider_volume->value()) << "," << QString::number(value);
    Player->setVolume(value);
    if(musicVolume_now != value)
    {
        musicVolume_last = musicVolume_now;
        musicVolume_now = value;
    }

    if(value == 0)
    {
        if(voiceOpenFlag == true)
        {
            ui->btn_voice->setIcon(QIcon(":/resource/voice_close.png" ));
        }
        voiceOpenFlag = false;
    }
    else
    {
        if(voiceOpenFlag == false)
        {
            ui->btn_voice->setIcon(QIcon(":/resource/voice_open.png" ));
        }
        voiceOpenFlag = true;
    }
    ui->lb_volume->setText(QString::number(value) + "%");
}

void MusicWindow::slot_voice_clicked(){
    if(voiceOpenFlag == false){
        ui->btn_voice->setIcon(QIcon(":/resource/voice_open.png" ));
        musicVolume_now = musicVolume_last;
        musicVolume_last = 0;
        voiceOpenFlag = true;
    }
    else{
        ui->btn_voice->setIcon(QIcon(":/resource/voice_close.png" ));
        musicVolume_last = musicVolume_now;
        musicVolume_now = 0;
        voiceOpenFlag = false;
    }
    Player->setVolume(musicVolume_now);
    ui->lb_volume->setText(QString::number(musicVolume_now) + "%");
}

// 获取文件总长度（当媒体文件发生变化时，用于获取总时长）
void MusicWindow::slot_DurationChanged(qint64 duration)
{
    // 将进度条最大值设定为文件总时长
    ui->slider_musicTime->setMaximum(duration);
    int secs =duration/1000;
    int mins = secs/60;
    secs = secs%60;
    totalTime = QString::asprintf("%d: %d",mins,secs);        // 总时长
    ui->label_totalDuration->setText(totalTime);
    //qDebug()<<totalTime;
}

// 播放进度发生变化时（当前媒体文件进度发生变化时，用于获取实时进度）
void MusicWindow:: slot_PositionChanged(qint64 position)
{
    ui->slider_musicTime->setValue(position);
    if(ui->slider_musicTime->isSliderDown())
    {
        return;
    }
    int secs = position/1000;
    int mins = secs/60;
    secs = secs%60;
    currTime = QString::asprintf("%d: %d",mins,secs);        // 当前播放时长
    ui->label_currDuration->setText(currTime);
}


//~~~~~~~~~~~~~~~ 播放拖拽、声音拖拽 结束 ~~~~~~~~~~~~~~~~
