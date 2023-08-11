#include "yolo.h"
#include <qstring.h>
#include<QObject>
#include <QDir>
#include<QCoreApplication>
YOLO::YOLO()
{
    //loadNet(false);
}

YOLO::~YOLO()
{

}

//std::vector<std::string> YOLO::load_class_list()
//{
//    std::vector<std::string> class_list;
//    std::ifstream ifs("G:/classes.txt");
//    std::string line;
//    while (getline(ifs, line))
//    {
//        class_list.push_back(line);
//    }
//    return class_list;
//}

//Net YOLO::loadNet(bool is_cuda)
//{
//    //F:\SVN-ZJKY\YiFeiShouJiRobot\yolov5-master
//    //m_net = readNet("F:\\SVN-ZJKY\\YiFeiShouJiRobot\\yolov5-master\\yolov5s.onnx");
//    m_net = readNet("F:\\SVN-ZJKY\\YiFeiShouJiRobot\\demo\\yolov5-opencv-dnn-cpp-main\\QtDemo\\build-yolov5-opencv-dnn-cpp-main-Qt5_6_2_MSVC2015_64bit-Release\\release\\yolov5s.onnx");
//    if (is_cuda)
//    {
//        cout << "Attempty to use CUDA\n";
//        m_net.setPreferableBackend(DNN_BACKEND_CUDA);
//        m_net.setPreferableTarget(DNN_TARGET_CUDA_FP16);
//    }
//    else
//    {
//        cout << "Running on CPU12\n";
//        m_net.setPreferableBackend(DNN_BACKEND_OPENCV);
//        m_net.setPreferableTarget(DNN_TARGET_CPU);
//    }

//    return m_net;
//}

//Mat YOLO::format_yolov5(const Mat &source)
//{
//    int col = source.cols;
//    int row = source.rows;
//    int _max = MAX(col, row);
//    Mat result = Mat::zeros(_max, _max, CV_8UC3);
//    source.copyTo(result(Rect(0, 0, col, row)));
//    return result;
//}

void YOLO::init(std::string onnxpath)
{
    //int i=cv::cuda::getCudaEnabledDeviceCount();
    //std::cout<<i<<std::endl;
    int i=cv::dnn::DNN_BACKEND_CUDA;
    int j=cv::dnn::DNN_TARGET_CUDA;
    std::cout<<i<<std::endl;
    std::cout<<j<<std::endl;
    net = cv::dnn::readNetFromONNX(onnxpath);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);


    QString relativePath = "res/classes.txt";

    // 获取当前应用程序所在的目录
    QString currentDir = QCoreApplication::applicationDirPath();

    // 拼接绝对路径
    QString absolutePath = QDir(currentDir).absoluteFilePath(relativePath);

    // 将路径转换为标准字符串
    std::string modelPath = absolutePath.toStdString();
    std::string file= modelPath;

    std::ifstream ifs(file);
    if (!ifs.is_open())
        CV_Error(cv::Error::StsError, "File " + file + " not found");
    std::string line;
    while (std::getline(ifs, line))
    {
        classes_.push_back(line);
    }
}

void YOLO::detect(cv::Mat & frame, std::vector<detect_result> &results)
{
    int w = frame.cols;
    int h = frame.rows;
    int _max = std::max(h, w);
    cv::Mat image = cv::Mat::zeros(cv::Size(_max, _max), CV_8UC3);
    cv::Rect roi(0, 0, w, h);
    frame.copyTo(image(roi));


    float x_factor = static_cast<float>(image.cols) / model_input_width_;
    float y_factor = static_cast<float>(image.rows) / model_input_height_;


    //std::cout<<x_factor<<std::endl;
    //std::cout<<y_factor<<std::endl;

    cv::Mat blob = cv::dnn::blobFromImage(image, 1 / 255.0, cv::Size(model_input_width_, model_input_height_), cv::Scalar(0, 0, 0), true, false);
    this->net.setInput(blob);
    cv::Mat preds = this->net.forward("output");//outputname


    cv::Mat det_output(preds.size[1], preds.size[2], CV_32F, preds.ptr<float>());

    std::vector<cv::Rect> boxes;
    std::vector<int> classIds;
    std::vector<float> confidences;
    for (int i = 0; i < det_output.rows; i++)
    {
        float box_conf = det_output.at<float>(i, 4);
        if (box_conf < nms_threshold_)
        {
            continue;
        }

        cv::Mat classes_confidences = det_output.row(i).colRange(5, 85);
        cv::Point classIdPoint;
        double cls_conf;
        cv::minMaxLoc(classes_confidences, 0, &cls_conf, 0, &classIdPoint);


        if (cls_conf > confidence_threshold_)
        {
            float cx = det_output.at<float>(i, 0);
            float cy = det_output.at<float>(i, 1);
            float ow = det_output.at<float>(i, 2);
            float oh = det_output.at<float>(i, 3);
            int x = static_cast<int>((cx - 0.5 * ow) * x_factor);
            int y = static_cast<int>((cy - 0.5 * oh) * y_factor);
            int width = static_cast<int>(ow * x_factor);
            int height = static_cast<int>(oh * y_factor);
            cv::Rect box;
            box.x = x;
            box.y = y;
            box.width = width;
            box.height = height;

            boxes.push_back(box);
            classIds.push_back(classIdPoint.x);
            confidences.push_back(cls_conf * box_conf);
        }
    }

    std::vector<int> indexes;
    cv::dnn::NMSBoxes(boxes, confidences, confidence_threshold_, nms_threshold_, indexes);
    for (size_t i = 0; i < indexes.size(); i++)
    {
        detect_result dr;
        int index = indexes[i];
        int idx = classIds[index];
        dr.box = boxes[index];
        dr.classId = idx;
        dr.confidence = confidences[index];
        results.push_back(dr);
    }
    QCoreApplication::processEvents();
}

void YOLO::draw_frame(cv::Mat & frame, std::vector<detect_result> &results,cv::Mat & target,QList<QTableWidgetItem*>& items)
{
    for(auto dr : results)
    {

        cv::rectangle(frame, dr.box , cv::Scalar(0, 0, 255), 2, 8);
        cv::rectangle(frame, cv::Point(dr.box .tl().x, dr.box .tl().y - 20), cv::Point(dr.box .br().x, dr.box .tl().y), cv::Scalar(255, 0, 0), -1);

        std::string label = cv::format("%.2f", dr.confidence);
        label = classes_[dr.classId ] + ":" + label;
        cv::putText(frame, label, cv::Point(dr.box.x, dr.box.y + 6), 1, 2, cv::Scalar(0, 255, 0),2);

        if(classes_[dr.classId]=="person"){
//               cv::Point pt1(dr.box .tl().x, dr.box .tl().y - 20);
//               cv::Point pt2(dr.box .br().x, dr.box .tl().y);
//               target =frame(cv::Rect(pt1, pt2));

            cv::Rect roi(dr.box);
            cv::Rect frame_rect(0, 0, frame.cols, frame.rows);
            cv::Rect clipped_rect = roi & frame_rect; //计算ROI和图像矩形的交集

            if (clipped_rect != roi) {
                //ROI已经被剪裁了，根据需要进行调整
                //例如，将ROI的宽度和高度截断到不超出图像的尺寸
                clipped_rect.width = std::min(clipped_rect.width, frame_rect.width - clipped_rect.x);
                clipped_rect.height = std::min(clipped_rect.height, frame_rect.height - clipped_rect.y);
            }

            target = frame(clipped_rect);

            //target=frame;
        }
        QString name=QString::fromStdString(classes_[dr.classId]);
        QTableWidgetItem *item = new QTableWidgetItem(name);
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(dr.confidence));
        QTableWidgetItem *item3 = new QTableWidgetItem(QString::number(dr.box.x));
        QTableWidgetItem *item4 = new QTableWidgetItem(QString::number(dr.box.x+dr.box.width));
        QTableWidgetItem *item5 = new QTableWidgetItem(QString::number(dr.box.y));
        QTableWidgetItem *item6 = new QTableWidgetItem(QString::number(dr.box.y+dr.box.height));

        items.append(item);
        items.append(item2);
        items.append(item3);
        items.append(item4);
        items.append(item5);
        items.append(item6);
    }
    QCoreApplication::processEvents();
}
