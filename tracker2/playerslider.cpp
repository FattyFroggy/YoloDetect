#include "playerslider.h"

PlayerSlider::PlayerSlider(QWidget * parent) : QSlider(parent)
{
    m_bPressed = false;// 是否允许进度条的进度设置
}
// 进度条的进度设置
void PlayerSlider::setProgress(qint64 i64Progress)
{
    if(!m_bPressed)
        setValue(i64Progress);
}
// 按下
void PlayerSlider::mousePressEvent(QMouseEvent *e)
{
    m_bPressed = true;//用户操作进度条时，不再让进度条响应QMediaPlaer设置的进度信息；
    QSlider::mousePressEvent(e);//必须有这句，否则手动不能移动滑块
}
// 移动
void PlayerSlider::mouseMoveEvent(QMouseEvent *e)
{
     QSlider::mouseMoveEvent(e);//必须有这句，否则手动不能移动滑块
}
// 抬起释放
void PlayerSlider::mouseReleaseEvent(QMouseEvent *e)
{
    m_bPressed = false;
    qint64 i64Pos = (qint64)value();
    emit sigProgress(i64Pos);// 发出位置改变信号
    QSlider::mouseReleaseEvent(e);//必须有这句，否则手动不能移动滑块
}
