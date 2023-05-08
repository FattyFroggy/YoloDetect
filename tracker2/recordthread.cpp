#include "recordthread.h"

RecordThread::RecordThread(QObject *parent)
    :QThread(parent)
{


}

void RecordThread::run()
{
    qDebug("startRec");
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyyMMdd");
    QString strDate = time.toString("yyyy-MM-dd-hhmmss");
//    QString timeStr1 = time.toString("mm-ss-zzz");

//    qDebug("readString = %s",qPrintable(timeStr1));

    QString fileStr = "D:/111/Video/"+strDate + ".avi";
    int fps=25;
    writer.open(fileStr.toStdString(),
                cv::VideoWriter::fourcc('X','2','6','4'),
                fps,
                cv::Size(640,480)
                );

    while (!isInterruptionRequested()) {


         QMutexLocker locker(&m_mutex);
         while (m_queue.isEmpty()) {
             if (!m_condition.wait(&m_mutex, 100)) { // 设置超时时间为100毫秒
                 qDebug("outtime");
                 qDebug("卡死结束录制");
                 writer.release();
                 return;
             }
         }

//         image=m_queue.dequeue();
//         frame=Qimag2Mat(image);

         frame=m_queue.dequeue();
         msleep(40); // 等待队列中有可用帧
         writer.write(frame);
        QCoreApplication::processEvents();
//        if (!m_queue.isEmpty()) {
//             //qDebug()<<m_queue.isEmpty();
//            //qDebug("出队开始");
//            image=m_queue.dequeue();
//            frame=Qimag2Mat(image);
//            //imshow("video",frame);

//            msleep(40); // 等待队列中有可用帧
//            writer.write(frame);
//            continue;
//        }
    }
//    QDateTime time2 = QDateTime::currentDateTime();
//    QString timeStr2 = time2.toString("mm-ss-zzz");

//    qDebug("readString2 = %s",qPrintable(timeStr2));
    qDebug("结束录制");
    writer.release();
}
void RecordThread::onRecord(const cv::Mat &mat)
{
    //qDebug("收到1帧");
    QMutexLocker locker(&m_mutex);
//    QImage ima=Mat2Qimag(mat);
    m_queue.enqueue(mat);
    m_condition.wakeAll();
}
cv::Mat RecordThread::Qimag2Mat(QImage image){


    cv::Mat mat;
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
        mat = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;

}
QImage RecordThread::Mat2Qimag(cv::Mat mat){

    // 8-bits unsigned, NO. OF CHANNELS = 1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    // 8-bits unsigned, NO. OF CHANNELS = 3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
