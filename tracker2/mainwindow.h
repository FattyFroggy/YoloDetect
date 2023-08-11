#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#if defined(_MSC_VER) && (_MSC_VER >= 1600)
# pragma execution_character_set("utf-8")
#endif
#include <QMainWindow>
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
#include<QtMaterialAppBar.h>
#include<QInputDialog>
#include<QDateTimeEdit>
#include<QMessageBox>
#include <QTableWidget>
#include <QStringList>
#include "widget.h"
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
    void on_OpenPicBtn_clicked();


    void on_OpenVideoWindow_clicked();

    void on_GetTracker(const cv::Mat &mat);

    void IntervalRec();
    void on_StopTimeRec_clicked();

    void on_SetTimeRec_clicked();
    void OnitemsEmitted(const QList<QTableWidgetItem*>& items);
    void OnGetFps(double &fps);
    void on_pushButton_clicked();

private:
    int row = 0;
    int col = 0;
    RecordThread *m_recordThread;
    VideoThread *m_videoThread;
//    PlayVideo *m_playVideoThread;
    cv::Mat frame;//用mat形式记录的图片
    cv::Mat recFrame;
    QImage imag;
    cv::VideoCapture cam;
    QTimer *timer;//定时器用于定时取帧，上面说的隔一段时间就去取就是用这个实现
    QTimer *EditTimer;
    QTimer *ButtonTimer;
//    QTimer *progressTimer;
    void init();

    bool isRec;
//    double rate;
    cv::VideoWriter writer;


};
#endif // MAINWINDOW_H
