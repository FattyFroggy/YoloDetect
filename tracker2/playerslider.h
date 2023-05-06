#ifndef PLAYERSLIDER_H
#define PLAYERSLIDER_H

#include <QObject>
#include <QSlider>
#include <QMouseEvent>

class PlayerSlider : public QSlider
{
    Q_OBJECT
signals:
    void sigProgress(qint64 i64Pos);

public:
    PlayerSlider(QWidget * parent = 0);

    bool    m_bPressed;
    void    setProgress(qint64);

    void    mousePressEvent(QMouseEvent *e)override;// 按下
    void    mouseMoveEvent(QMouseEvent *e)override;// 移动
    void    mouseReleaseEvent(QMouseEvent *e)override;// 抬起释放
};

#endif // PLAYERSLIDER_H
