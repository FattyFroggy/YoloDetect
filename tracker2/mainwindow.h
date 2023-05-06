#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <unistd.h>
#include <iostream>
#include <QDebug>
#include <QLabel>
#include <QImage>
#include <vector>
#include<QTimer>
#include <QPixmap>
#include <QDateTime>
#include <QDebug>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <QMediaPlayer>
#include<QFileDialog>
#include <QVideoWidget>
#include "videothread.h"
#include "recordthread.h"
#include<QProgressBar>
#include<playermainwindow.h>
using namespace std;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QImage Mat2QImage(const cv::Mat &mat);
    Ui::MainWindow *ui;
    ~MainWindow();


signals:
    void sendRecord(const cv::Mat &mat);
private slots:
    void handleError(const QString& error);
    void ShowCameraLabel(const cv::Mat &frame);
    void on_OpenCameraBtn_clicked();

    void on_TakePicBtn_clicked();

    void on_CloseCameraBtn_clicked();

    void readFrame();//读取一帧

    void on_SavePicBtn_clicked();

    void on_StartRecBtn_clicked();

    void on_EndRecBtn_clicked();
    
    void onRec();
    void checkImage(cv::Mat &img,cv::Mat &img_2);
    void on_OpenPicBtn_clicked();

//    void updateFrame(const QImage& frame, int frameNo);
//    void updateDuration(int duration);
//    void seek(int frameNo);
    void on_OpenVideoWindow_clicked();

private:
//    QProgressBar *progressBar;
//    QVideoWidget *videoWidget;// 视频显示组件
//    QMediaPlayer * player;//媒体播放器类
    RecordThread *m_recordThread;
    VideoThread *m_videoThread;
//    PlayVideo *m_playVideoThread;
    cv::Mat frame;//用mat形式记录的图片
    cv::Mat recFrame;
    QImage imag;
    cv::VideoCapture cam;
    QTimer *timer;//定时器用于定时取帧，上面说的隔一段时间就去取就是用这个实现
//    QTimer *progressTimer;
    void init();

    bool isRec;
//    double rate;
    cv::VideoWriter writer;

};
#endif // MAINWINDOW_H
