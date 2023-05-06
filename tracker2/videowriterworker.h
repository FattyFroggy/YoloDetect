#ifndef VIDEOWRITERWORKER_H
#define VIDEOWRITERWORKER_H

#include <QWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QThread>
#include <QTimer>
#include <QCloseEvent>
#include <QDebug>
#include <opencv2/opencv.hpp>

namespace Ui {
class VideoWriterWorker;
}

class VideoWriterWorker : public QWidget
{
    Q_OBJECT

public:
    explicit VideoWriterWorker(QWidget *parent = nullptr);
    ~VideoWriterWorker();

private:
    cv::video


};

#endif // VIDEOWRITERWORKER_H
