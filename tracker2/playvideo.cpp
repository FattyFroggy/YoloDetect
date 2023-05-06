#include "playvideo.h"

PlayVideo::PlayVideo(QObject *parent)
{
    isStopped = false;
    isPaused = false;
    frameRate = 25;
    filePath = "";
    videoWidget = nullptr;

}
void PlayVideo::startPlay(QString filePath, QVideoWidget *videoWidget)
{
    qDebug("11");
    QMutexLocker locker(&mutex);
    this->filePath = filePath;
    this->videoWidget = videoWidget;
    isStopped = false;
    isPaused = false;
    condition.wakeAll();
}

void PlayVideo::pause()
{
    QMutexLocker locker(&mutex);
    isPaused = !isPaused;
    if (!isPaused) {
        condition.wakeAll();
    }
}

void PlayVideo::stop()
{
    QMutexLocker locker(&mutex);
    isStopped = true;
    isPaused = false;
    condition.wakeAll();
}

QImage PlayVideo::getCurrentFrame()
{
    QMutexLocker locker(&mutex);
    return currentFrame;
}

int PlayVideo::getFrameRate()
{
    QMutexLocker locker(&mutex);
    return frameRate;
}
void PlayVideo::seek(double progress)
{
    QMutexLocker locker(&mutex);
    if (isPaused) {
        this->progress = progress;
        condition.wakeAll();
    }
}
void PlayVideo::run()
{
    qDebug("22");
    cv::VideoCapture capture;
    if (!capture.open(filePath.toStdString())) {
        emit durationChanged(0);
        emit frameChanged(QImage(), 0);
        return;
    }

    double totalFrames = capture.get(cv::CAP_PROP_FRAME_COUNT);
    int width = capture.get(cv::CAP_PROP_FRAME_WIDTH);
    int height = capture.get(cv::CAP_PROP_FRAME_HEIGHT);
    frameRate = capture.get(cv::CAP_PROP_FPS);
    emit durationChanged(static_cast<int>(totalFrames / frameRate));

    while (!isStopped) {
        if (isPaused) {
            QThread::msleep(100);
            continue;
        }

        cv::Mat frame;
        if (!capture.read(frame)) {
            break;
        }

        cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
        QImage image(frame.data, frame.cols, frame.rows, static_cast<int>(frame.step), QImage::Format_RGB888);
        currentFrame = image;

        if (videoWidget != nullptr) {
            videoWidget->repaint();
        }

        emit frameChanged(image, static_cast<int>(capture.get(cv::CAP_PROP_POS_FRAMES)));

        QThread::msleep(static_cast<unsigned long>(1000 / frameRate));
    }

    capture.release();
}
