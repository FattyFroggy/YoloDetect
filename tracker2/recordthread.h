#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H

#include <QThread>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <QMutex>
#include<QSemaphore>
#include<QDebug>
#include <QDateTime>
#include<QApplication>
#include <QQueue>
#include<QImage>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/imgproc/types_c.h>
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <QtCore>

class RecordThread:public QThread
{
      Q_OBJECT
public:
    RecordThread(QObject *parent = nullptr);

   void run() override;
   cv::Mat Qimag2Mat(QImage image);
   QImage Mat2Qimag(cv::Mat mat);
   bool isRec=false;
public slots:
   void onRecord(const cv::Mat &mat);
private:
   cv::VideoWriter writer;
   QQueue<QImage>  m_queue;
   cv::Mat frame;
   QImage image;
   QWaitCondition m_condition;
   QMutex m_mutex;
};

#endif // RECORDTHREAD_H
