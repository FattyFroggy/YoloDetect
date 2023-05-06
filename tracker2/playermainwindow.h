#ifndef PLAYERMAINWINDOW_H
#define PLAYERMAINWINDOW_H

#include <QWidget>
#include <QtMultimedia>
#include <QVideoWidget> // 视频显示组件窗口
#include"playerslider.h"
#include <QMainWindow>
#include<QFileDialog>
namespace Ui {
class PlayerMainWindow;
}

class PlayerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit PlayerMainWindow(QWidget *parent = nullptr);
    ~PlayerMainWindow();
    bool m_bReLoad;// 已经载入还没设置进度条最大值
signals:
       void back();
private:
    Ui::PlayerMainWindow *ui;
    QVideoWidget *m_pPlayerWidget;// 视频显示组件
    QMediaPlayer * m_pPlayer;//媒体播放器类
public slots:
    void    OnSetMediaFile(void);//载入
    void    OnSlider(qint64); // 信号内容的位置已更改为位置，以毫秒表示，进度条也要变
    void    OnDurationChanged(qint64);// 信号表示内容的持续时间已更改为时长，以毫秒表示，进度条 的最大值和最小值
    void    OnStateChanged(QMediaPlayer::State);// Player对象的状态已经改变
private slots:
    void on_BackToMain_clicked();
};

#endif // PLAYERMAINWINDOW_H
