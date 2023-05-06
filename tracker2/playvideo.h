#ifndef PLAYVIDEO_H
#define PLAYVIDEO_H
#include <QObject>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QImage>
#include <QVideoWidget>
#include <opencv2/opencv.hpp>
class PlayVideo:public QThread
{
    Q_OBJECT
public:
    PlayVideo(QObject *parent = nullptr);
    void startPlay(QString filePath, QVideoWidget *videoWidget);
    void pause();
    void stop();
    QImage getCurrentFrame();
    int getFrameRate();
    void seek(double progress);

signals:
    void durationChanged(double duration);
    void frameChanged(QImage frame, int frameNumber);
protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;
    bool isStopped;
    bool isPaused;
    double frameRate;
    QString filePath;
    QVideoWidget *videoWidget;
    QImage currentFrame;
    double progress;
};

#endif // PLAYVIDEO_H
