#include "playermainwindow.h"
#include "ui_playermainwindow.h"

PlayerMainWindow::PlayerMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerMainWindow)
{
    ui->setupUi(this);
    setWindowTitle("播放器");
    setWindowIcon(QIcon(":/icon/icon.webp"));// 图标
    setFixedSize(800,600);//设置固定大小

    // 1.放置播放窗口
    m_pPlayer = new QMediaPlayer;//媒体播放器类
    m_pPlayerWidget = new QVideoWidget;// 视频显示组件窗口
    m_pPlayer->setVideoOutput(m_pPlayerWidget);//视频输出:设置视频输出，允许用户将视频渲染到自定义的视频输出设备上。
    ui->verticalLayout->addWidget(m_pPlayerWidget);//将视频显示组件窗口添加到QVBoxLayout垂直布局

    // 设置视频小部件是否自动填充背景。true则视频小部件将自动填充背景，以便在视频播放期间保持视觉效果。
    m_pPlayerWidget->setAutoFillBackground(true);

    // 2.界面美化
    QPalette qplte;// 调色板
    qplte.setColor(QPalette::Window, QColor(0,0,0));// 透明
    m_pPlayerWidget->setPalette(qplte);// 设置窗口部件的调色板

    // 3. 槽函数
    //载入
    connect(ui->BtnLoad, SIGNAL(clicked()), this, SLOT(OnSetMediaFile()));
    //播放
    connect(ui->BtnPlay, SIGNAL(clicked()), m_pPlayer, SLOT(play()));// 对QMediaPlayer媒体播放器操作
    //停止
    connect(ui->BtnStop, SIGNAL(clicked()), m_pPlayer, SLOT(pause()));// 对QMediaPlayer媒体播放器操作

    // Player对象的状态已经改变。
    connect(m_pPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(OnStateChanged(QMediaPlayer::State)));

    ui->BtnStop->setEnabled(false);// 停止：默认不可用
    //设置滑块行为
    m_bReLoad = true;
    ui->slider->setEnabled(false);// 进度条：默认不可用

    // 信号内容的位置已更改为位置，以毫秒表示，进度条也要变
    connect(m_pPlayer, SIGNAL(positionChanged(qint64)), this, SLOT(OnSlider(qint64)));
    // 信号表示内容的持续时间已更改为时长，以毫秒表示，第一次为进度条 的最大值和最小值
    connect(m_pPlayer, SIGNAL(durationChanged(qint64)), this, SLOT(OnDurationChanged(qint64)));
    // 进度条：鼠标移动位置;setPosition()设置当前播放的位置,实现跳转播放的功能
    connect(ui->slider, SIGNAL(sigProgress(qint64)), m_pPlayer, SLOT(setPosition(qint64)));

}

PlayerMainWindow::~PlayerMainWindow()
{
    qDebug("删除窗口");
    delete m_pPlayer;
    delete m_pPlayerWidget;
    delete ui;
}
//载入
void PlayerMainWindow::OnSetMediaFile()
{
    QFileDialog dialog(this);//文件选择窗口
    dialog.setFileMode(QFileDialog::AnyFile);//设置文件模式(文件/文件夹)：任意文件，无论是否存在
    QStringList fileNames;
    if (dialog.exec())
        fileNames = dialog.selectedFiles();// 存所有选择的文件

    if(!fileNames.empty())
    {
        qDebug()<<"文件名:"<<fileNames[0];
        m_pPlayer->setMedia(QUrl::fromLocalFile(fileNames[0]));// 设置媒体信息
        m_bReLoad = true;
        ui->slider->setValue(0);//进度条数字在0(开始位置)
        setWindowTitle(fileNames[0]);
    }
}
// 信号内容的位置已更改为位置，以毫秒表示，进度条也要变
void PlayerMainWindow::OnSlider(qint64 i64Pos)
{
    // 自定义的位置移动==setValue(i64Pos);
    ui->slider->setProgress(i64Pos);
}
// 信号表示内容的持续时间已更改为时长，以毫秒表示，进度条 的最大值和最小值
void PlayerMainWindow::OnDurationChanged(qint64 i64Duration)
{
    // 第一次i64Duration 为最大值
    if(i64Duration > 0 && m_bReLoad) // 最大值>0并且 已经载入还没设置进度条最大值
    {
        ui->slider->setRange(0, i64Duration);

        ui->minTime->setText(QString::number(0));
        QTime time = QTime::fromMSecsSinceStartOfDay(i64Duration);
        ui->maxTime->setText(time.toString("hh:mm:ss:zzz"));

        m_bReLoad = false;// 不能再设置进度条最大值
    }
}
// Player对象的状态已经改变
void PlayerMainWindow::OnStateChanged(QMediaPlayer::State enumState)
{
    if(QMediaPlayer::StoppedState == enumState)//停止
    {
        ui->BtnPlay->setEnabled(true);
        ui->BtnStop->setEnabled(false);
        ui->slider->setEnabled(false);
    }
    else if(QMediaPlayer::PlayingState == enumState)// 播放
    {
        //ui->BtnPlay->setEnabled(false);
        ui->BtnStop->setEnabled(true);
        ui->slider->setEnabled(true);
    }
}


void PlayerMainWindow::on_BackToMain_clicked()
{
    emit back();
}
