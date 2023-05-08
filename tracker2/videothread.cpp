#include "videothread.h"

VideoThread::VideoThread(QObject *parent)
    : QThread(parent)
{

}

void VideoThread::on_GetTracker(const cv::Mat &mat){
    emit SendTracker(mat);
};
void VideoThread::run()
{
    m_capture.open(0,cv::CAP_DSHOW);
    if(!m_capture.isOpened())
    {

        return;
    }
     // 读取第一帧图像，并将其转换为灰度图像
    cv::Mat frame;
    m_capture.read(frame);
    if(frame.empty())
        return;

    YOLO *yolov5 = new YOLO;
    yolov5->init("C:/Users/daras/Downloads/yolov5_onnx_dnn-master/yolov5_onnx_dnn-master/c++/yolov5s.onnx");

    std::vector<detect_result> output;
    int total_frames = 0;

    clock_t start_name = 0;
    clock_t end_time = 0;
    clock_t end_time2 = 0;
    cvtColor(frame, m_previousFrame, cv::COLOR_BGR2GRAY);
    while(!isInterruptionRequested())
    {

        start_name = clock();
        m_capture.read(frame);
        if (frame.empty())
        {
            std::cout << "End of stream\n";
            break;
        }
        total_frames++;
        yolov5->detect(frame,output);

        end_time=clock();
        //int i=(40-(end_time-start_name));
        //msleep();
        int duration = ((double)(end_time - start_name) / CLOCKS_PER_SEC)*1000;
        if(duration<40){
            msleep(40-duration);
        }
        //std::cout<<duration<<std::endl;

        end_time2=clock();
        //std::cout<<(1 / ((double)(end_time2 - start_name) / CLOCKS_PER_SEC))<<std::endl;
        auto text = "FPS: " + std::to_string(1 / ((double)(end_time2 - start_name) / CLOCKS_PER_SEC));
        //qDebug() << "Frame time(ms): " << (double)(end_time - start_name) /*/ CLOCKS_PER_SEC*/;
        cv::putText(frame, text, cv::Point(3, 25), cv::FONT_ITALIC, 0.8, cv::Scalar(0, 0, 255), 2);
        cv::Mat target;
        yolov5->draw_frame(frame,output,target);
        emit SendTracker(target);
        //msleep(i);


        emit CameraLabel(frame);

        if (isInterruptionRequested()){
            std::cout << "finished by user\n";
            break;
        }
        output.clear();


    }
    m_capture.release();
}








//m_capture.open(0);
//if(!m_capture.isOpened())
//{

//    return;
//}
//qDebug()<<QString::number(m_capture.get(cv::CAP_PROP_FRAME_WIDTH));
// qDebug()<<QString::number(m_capture.get(cv::CAP_PROP_FRAME_HEIGHT));
// // 读取第一帧图像，并将其转换为灰度图像
//cv::Mat frame;
//m_capture.read(frame);
//if(frame.empty())
//    return;
//cvtColor(frame, m_previousFrame, cv::COLOR_BGR2GRAY);
//while(!isInterruptionRequested())
//{
//    cv::Mat frame;
//    m_capture.read(frame);
//    if(frame.empty())
//        return;

//    // 将当前帧图像转换为灰度图像
//    cv::Mat currentFrame;
//    cvtColor(frame, currentFrame, cv::COLOR_BGR2GRAY);

//    // 计算两帧之间的差异
//    cv::Mat diff;
//    absdiff(m_previousFrame, currentFrame, diff);

//    // 对差异图像进行二值化
//    cv::Mat binary;
//    threshold(diff, binary, 60  , 255, cv::THRESH_BINARY);

//    // 对二值化后的图像进行膨胀和腐蚀操作
//    cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
//    erode(binary, binary, element);
//    element = getStructuringElement(cv::MORPH_RECT,cv:: Size(25, 25));
//    dilate(binary, binary, element);

//    // 进行连通区域分析，找到所有的连通区域，并将其包围矩形框绘制在原始图像上
//    std::vector<std::vector<cv::Point>> contours;
//    findContours(binary, contours, cv::RETR_EXTERNAL,cv:: CHAIN_APPROX_SIMPLE,cv::Point(0,0));
//    for(size_t i = 0; i < contours.size(); i++)
//    {
//        cv::Rect rect = boundingRect(contours[i]);
//        rectangle(frame, rect, cv::Scalar(0, 255, 0), 2,8,0);
//    }

//    emit CameraLabel(frame);

//    //imshow("AABB",frame);
//    cv::waitKey(30);
//    m_previousFrame = currentFrame;

//     //qDebug("未结束");
//    if (isInterruptionRequested()){
//      qDebug("结束");
//       break;
//    }


//}
//m_capture.release();
//读取两帧图像，并将它们转换成灰度图像。

//计算两帧之间的差异，可以使用 absdiff 函数来计算两个图像的差异。

//对差异图像进行二值化，可以使用 threshold 函数来进行二值化，将差异图像中的较大值设为 255，较小值设为 0。

//对二值化后的图像进行膨胀和腐蚀操作，可以使用 dilate 和 erode 函数来进行膨胀和腐蚀操作，以去除噪声和孤立点。

//进行连通区域分析，找到二值化后的图像中所有的连通区域，并将其包围矩形框绘制在原始图像上。
