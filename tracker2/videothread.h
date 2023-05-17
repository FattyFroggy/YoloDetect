#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include<QSemaphore>
#include<QDebug>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "yolo.h"
#include <QList>
#include <QTableWidgetItem>

class VideoThread : public QThread
{
    Q_OBJECT
public:
    VideoThread(QObject *parent = nullptr);

    void run() override;
    void on_GetTracker(const cv::Mat &mat);


signals:
    void error(const QString& msg);
    void CameraLabel(const cv::Mat &frame);

    void captureReady(cv::VideoCapture *capture);

    void SendTracker(const cv::Mat &mat);

    void itemsEmitted(const QList<QTableWidgetItem*>& items);
    void SendFps(double &fps);
private:
    QList<QTableWidgetItem*> items;
    QMutex m_mutex;
    cv::VideoCapture m_capture;

    cv::Mat m_previousFrame;

    QSemaphore m_semaphore;
};

#endif // VIDEOTHREAD_H
