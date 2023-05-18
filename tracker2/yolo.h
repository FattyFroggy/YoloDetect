#include <fstream>
#include <sstream>
#include <iostream>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <QObject>
//#include<opencv2/cudalegacy.hpp>
#include <QList>
#include <QTableWidgetItem>
using namespace std;

class detect_result
{

public:
    int classId;
    float confidence;
    cv::Rect_<float> box;
};

class YOLO
{
public:
    YOLO();
    ~YOLO();
    void init(std::string onnxpath);
    //Net loadNet(bool is_cuda);
    //Mat format_yolov5(const Mat &source);
    std::vector<std::string> classes_;
    void detect(cv::Mat & frame, std::vector<detect_result> &result);
    //void drawRect(Mat &image,vector<Detection> &output);
    //std::vector<std::string> load_class_list();
    void draw_frame(cv::Mat & frame, std::vector<detect_result> &results,cv::Mat & target,QList<QTableWidgetItem*>& items);
private:
    cv::dnn::Net net;
    //cv::dnn::Net m_net;
//    const float INPUT_WIDTH = 640.0;
//    const float INPUT_HEIGHT = 640.0;
//    const float SCORE_THRESHOLD = 0.2;
//    const float NMS_THRESHOLD = 0.4;
//    const float CONFIDENCE_THRESHOLD = 0.4;


    const float confidence_threshold_ =0.25f;
    const float nms_threshold_ = 0.4f;
    const int model_input_width_ = 640;
    const int model_input_height_ = 640;
};
