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

class VideoThread : public QThread
{
    Q_OBJECT
public:
    VideoThread(QObject *parent = nullptr);

    void run() override;



signals:
    void error(const QString& msg);
    void CameraLabel(const cv::Mat &frame);

    void captureReady(cv::VideoCapture *capture);
private:

    QMutex m_mutex;
    cv::VideoCapture m_capture;

    cv::Mat m_previousFrame;

    QSemaphore m_semaphore;
};

#endif // VIDEOTHREAD_H
