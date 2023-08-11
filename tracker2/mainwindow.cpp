#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"qtmaterialautocomplete.h"

using namespace  cv;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    m_pPlayer = new QMediaPlayer;//媒体播放器类
//    m_pPlayerWidget = new QVideoWidget;// 视频显示组件窗口
//    m_pPlayer->setVideoOutput(m_pPlayerWidget);//视频输出:设置视频输出，允许用户将视频渲染到自定义的视频输出设备上。
//    ui->verticalLayout_2->addWidget(m_pPlayerWidget);//将视频显示组件窗口添加到QVBoxLayout垂直布局
//    // 设置视频小部件是否自动填充背景。true则视频小部件将自动填充背景，以便在视频播放期间保持视觉效果。
//    m_pPlayerWidget->setAutoFillBackground(true);
//    // 2.界面美化
//    QPalette qplte;// 调色板
//    qplte.setColor(QPalette::Window, QColor(0,0,0));// 透明
//    m_pPlayerWidget->setPalette(qplte);// 设置窗口部件的调色板
//    m_pPlayerWidget->show();

    // 初始化 QMediaPlayer 和 QVideoWidget
   // player = new QMediaPlayer(this);
//    videoWidget = new QVideoWidget(this);
//    videoWidget = new QVideoWidget(this);
//    ui->centralwidget->layout()->addWidget(videoWidget);

//    progressBar = new QProgressBar(this);
//    ui->statusbar->addWidget(progressBar);



    //ui->progressBar->setMaximum(100);




    //加载样式表
    QString qss;
    QFile file(":/css/blue.css");

    if (file.open(QFile::ReadOnly))
    {
        //用readAll读取默认支持的是ANSI格式,如果不小心用creator打开编辑过了很可能打不开
        qss = QLatin1String(file.readAll());

        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(QColor(paletteColor)));
        qApp->setStyleSheet(qss);
        file.close();
    }


    /*皮肤设置*/
    QFile file2(":/res/qss/style-4.qss");/*QSS文件所在的路径*/
    file2.open(QFile::ReadOnly);
    QTextStream filetext(&file2);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file2.close();

    QStringList horizontalHeaders;
    horizontalHeaders << "class" << "confirm" << "x1"<< "x2"<< "y1"<< "y2";
    ui->tableWidget->setColumnCount(horizontalHeaders.count());
    ui->tableWidget->setRowCount(10);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setHorizontalHeaderLabels(horizontalHeaders);



   init();



}

MainWindow::~MainWindow()
{

    disconnect(m_videoThread); 
    if(!m_videoThread->isFinished()){
        on_CloseCameraBtn_clicked();
    }
    disconnect(m_recordThread);
    if(!m_recordThread->isFinished()){
        on_EndRecBtn_clicked();
    }
    destroyAllWindows();
    delete m_videoThread;
    delete m_recordThread;
    delete ui;


}

void MainWindow::init()
{

//    m_playVideoThread = new PlayVideo(this);
//    connect(m_playVideoThread, &PlayVideo::frameChanged, this, &MainWindow::updateFrame);
//    connect(m_playVideoThread, &PlayVideo::durationChanged, this, &MainWindow::updateDuration);
//    connect(ui->horizontalSlider, &QSlider::valueChanged, this, &MainWindow::seek);

    m_videoThread = new VideoThread(this);
    connect(m_videoThread, &VideoThread::error, this, &MainWindow::handleError);
    connect(m_videoThread, &VideoThread::CameraLabel, this, &MainWindow::ShowCameraLabel,Qt::DirectConnection);
    connect(m_videoThread, &VideoThread::SendTracker, this, &MainWindow::on_GetTracker,Qt::DirectConnection);
    connect(m_videoThread, &VideoThread::itemsEmitted, this, &MainWindow::OnitemsEmitted,Qt::DirectConnection);
    connect(m_videoThread, &VideoThread::SendFps, this, &MainWindow::OnGetFps,Qt::DirectConnection);
    timer=new QTimer(this);
    //progressTimer=new QTimer(this);

    m_recordThread = new RecordThread(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::onRec,Qt::DirectConnection);
    connect(this,&MainWindow::sendRecord,m_recordThread,&RecordThread::onRecord,Qt::DirectConnection);

    ui->OpenCameraBtn->setEnabled(true);
    ui->CloseCameraBtn->setEnabled(false);
    ui->TakePicBtn->setEnabled(false);
    ui->SavePicBtn->setEnabled(false);
    ui->StartRecBtn->setEnabled(false);
    ui->EndRecBtn->setEnabled(false);
    ui->StopTimeRec->setEnabled(false);


    ui->label_24->setText("未定时");
    ui->label_20->setText("未录制");
    ui->dateTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    ui->dateTimeEdit->setMinimumDateTime(QDateTime::currentDateTime());


   ButtonTimer=new QTimer(this);
   connect(ButtonTimer, &QTimer::timeout, this, &MainWindow::on_pushButton_clicked,Qt::DirectConnection);
    ButtonTimer->setInterval(1000);//
    ButtonTimer->start();


    qRegisterMetaType<QVector<int>>();
    qRegisterMetaType<Qt::Orientation>();
}
void MainWindow::on_GetTracker(const cv::Mat &target){
    imag = Mat2QImage(target);

    if(imag.isNull()){
        //ui->label_3->clear();
        ui->label_2->clear();
    }else{
        //ui->label_2->clear();
        ui->label_2->setPixmap(QPixmap::fromImage(imag.scaled(ui->label->size(),Qt::KeepAspectRatio)));
        //ui->label_3->setText("出现可疑人物！！！！");
    }

    QCoreApplication::processEvents();

};
void MainWindow::handleError(const QString& error)
{
    //ui->statusbar->showMessage(error);
}
//void MainWindow::updateFrame(const QImage& frame, int frameNo)
//{

//    ui->videoWidget->setPixmap(QPixmap::fromImage(frame));
//    ui->horizontalSlider->setValue(frameNo);
//}
//void MainWindow::updateDuration(int duration)
//{
//    ui->horizontalSlider->setRange(0, duration);
//    ui->durationLabel->setText(QString("%1").arg(duration));
//}
//void MainWindow::seek(int frameNo)
//{
//    //m_playVideoThread->seek(frameNo);
//}
void MainWindow::onRec()
{
    emit sendRecord(recFrame);
    QCoreApplication::processEvents();
}
void MainWindow::OnitemsEmitted(const QList<QTableWidgetItem*>& items)
{
      row = 0;
      col = 0;

    ui->tableWidget->clear();
    QCoreApplication::processEvents();
    QStringList horizontalHeaders;
    horizontalHeaders << "class" << "confirm" << "x1"<< "x2"<< "y1"<< "y2";
    ui->tableWidget->setHorizontalHeaderLabels(horizontalHeaders);
    QCoreApplication::processEvents();
    for (const QTableWidgetItem* item : items)
    {
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::fromStdString(item->text().toStdString()));
        QString text=newItem->text();
        ui->tableWidget->setItem(row, col, newItem);
        col++;
        QCoreApplication::processEvents();
        // 如果达到行的最大列数，增加行数并重置列数为0
        if (col >= ui->tableWidget->columnCount()) {
            row++;
            col = 0;
        }
    }
    QCoreApplication::processEvents();
}
void MainWindow::OnGetFps(double &fps){
    ui->label_22->setText(QString::number(fps));
    QCoreApplication::processEvents();
}
void MainWindow::ShowCameraLabel(const Mat &frame)
{

    if(frame.empty()){
        //ui->label_3->clear();
        qDebug("播放中");
    }else

    recFrame=frame;
    //cv::imshow("video",frame);
    imag = Mat2QImage(frame);
    //ui->label->clear();
    //qDebug("播放中");
    ui->label->setPixmap(QPixmap::fromImage(imag.scaled(ui->label->size(),Qt::KeepAspectRatio)));


    QDateTime time = QDateTime::currentDateTime();
    QString Date = time.toString("yyyy-MM-dd hh:mm:ss");
    ui->label_4->setText(Date);
    //qDebug("播放中2");
    QCoreApplication::processEvents();
}
QImage MainWindow::Mat2QImage(const Mat &mat)//mat转QImage
{
    if(mat.type()==CV_8UC1)
    {
        QVector<QRgb>colorTable;
        for(int i=0;i<256;i++)
            colorTable.push_back(qRgb(i,i,i));
        const uchar*qImageBuffer=(const uchar*)mat.data;
        QImage img(qImageBuffer,mat.cols,mat.rows,mat.step,QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    //8-bitsunsigned,NO.OFCHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        const uchar*qImageBuffer=(const uchar*)mat.data;
        QImage img(qImageBuffer,mat.cols,mat.rows,mat.step,QImage::Format_RGB888);
        return  img.rgbSwapped();
    }
    else
    {
        qDebug()<<"ERROR:MatcouldnotbeconvertedtoQImage.";
        return QImage();
    }
    QCoreApplication::processEvents();
}



void MainWindow::on_OpenCameraBtn_clicked()
{

    ui->OpenCameraBtn->setEnabled(false);
    ui->CloseCameraBtn->setEnabled(true);
    ui->StartRecBtn->setEnabled(true);
    ui->TakePicBtn->setEnabled(true);
    ui->SavePicBtn->setEnabled(true);
    m_videoThread->start();

}

void MainWindow::on_TakePicBtn_clicked()
{

    imag = ui->label->pixmap()->toImage();
    ui->label_3->setPixmap(QPixmap::fromImage(imag.scaled(ui->label_3->size(),Qt::KeepAspectRatio)));

}

void MainWindow::on_CloseCameraBtn_clicked()
{
    if(!m_recordThread->isFinished()){
        on_EndRecBtn_clicked();
    }

    ui->TakePicBtn->setEnabled(false);
    ui->SavePicBtn->setEnabled(false);
    ui->OpenCameraBtn->setEnabled(true);
    ui->CloseCameraBtn->setEnabled(false);
    ui->StartRecBtn->setEnabled(false);
    ui->EndRecBtn->setEnabled(false);


    m_videoThread->requestInterruption();
    m_videoThread->wait();

    cout<<m_videoThread->isFinished()<<endl;
    ui->label->clear();
    ui->label_3->clear();
    ui->label_2->clear();

    //delete m_videoThread;
}

void MainWindow::readFrame()
{
    cam>>frame;
    imag = Mat2QImage(frame);
    ui->label->setPixmap(QPixmap::fromImage(imag.scaled(ui->label->size(),Qt::KeepAspectRatio)));
}



void MainWindow::on_SavePicBtn_clicked()
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("yyyyMMdd");
    QString strDate = time.toString("yyyy-MM-dd-hhmmss");
    //QString fileStr = "C://Users//daras//Pictures//Camera Roll//"+strDate + ".png";
    QString fileStr = "D:/111/"+strDate;

    QString fileName = QFileDialog::getSaveFileName(this, "Save Image", fileStr, "PNG Images (*.png);;JPEG Images (*.jpg);;Bitmap Images (*.bmp)");
    //QString fileStr = "D:/111/Images"+strDate + fileName;


    QString readString = fileName;
    qDebug("readString = %s",qPrintable(readString));

    if(imag.save(readString)){
          // ui->label_state->setText("图片保存成功");

           QMessageBox::information(NULL, "savePic", "保存截图成功", QMessageBox::Yes);
     }else {
           QMessageBox::warning(NULL,"Error","保存截图失败");
    }
}

void MainWindow::on_StartRecBtn_clicked()
{
    ui->StartRecBtn->setEnabled(false);
    ui->EndRecBtn->setEnabled(true);
    timer->start(40);
    m_recordThread->start();
    ui->label_20->setText("录制中......");

}



void MainWindow::on_EndRecBtn_clicked()
{
    qDebug("按下结束录制");
    ui->StartRecBtn->setEnabled(true);
    ui->EndRecBtn->setEnabled(false);
    timer->stop();
    m_recordThread->requestInterruption();
    m_recordThread->wait();
    ui->label_20->setText("未录制");

}


void MainWindow::on_OpenPicBtn_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "D:/111/Images", tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        cv::Mat image = cv::imread(fileName.toStdString(), cv::IMREAD_COLOR);

        if (image.empty()) {
            qDebug() << "Failed to open image file";
            return;
        }
        QImage ShwoPic=Mat2QImage(image);
         ui->label_3->setPixmap(QPixmap::fromImage(ShwoPic.scaled(ui->label->size(),Qt::KeepAspectRatio)));
    }
}
void MainWindow::on_OpenVideoWindow_clicked()
{
    PlayerMainWindow *p=new PlayerMainWindow();
    this->hide();
    p->show();

    connect(p,&PlayerMainWindow::back,[=](){
         p->disconnect(p);
         p->close();
        this->show();
    });

}

//void MainWindow::on_OpenVideoBtn_clicked()
//{
//    // 弹出文件选择对话框，选择要打开的视频文件
//    QString fileName = QFileDialog::getOpenFileName(this, "打开视频文件", "D:/111/", "视频文件 (*.mp4 *.avi)");

//    // 如果选择了视频文件，则启动视频线程并开始播放视频
//    if (!fileName.isEmpty()) {
//        m_playVideoThread->startPlay(fileName,videoWidget);
//    }
//}

//void MainWindow::on_VideoPauseBtn_clicked()
//{
//    m_playVideoThread->pause();
//}


//void MainWindow::on_VideoStopBtn_clicked()
//{
//    m_playVideoThread->stop();
//    progressTimer->stop();

//    //ui->progressBar->setValue(0);
//    //ui->currentFrameLabel->setText("0");
//}

void MainWindow::IntervalRec()
{
    qDebug() << EditTimer->remainingTime();
    qDebug() << EditTimer->interval();
    qDebug("IntervalRec");
    if(ui->StartRecBtn->isEnabled())
    {
        this->on_StartRecBtn_clicked();
    }else{
        this->on_EndRecBtn_clicked();
        this->on_StartRecBtn_clicked();
    }
    QCoreApplication::processEvents();
};

void MainWindow::on_SetTimeRec_clicked()
{
//    ui->OpenCameraBtn->setEnabled(false);
//    ui->CloseCameraBtn->setEnabled(false);
//    ui->TakePicBtn->setEnabled(false);
//    ui->SavePicBtn->setEnabled(false);
//    ui->StopTimeRec->setEnabled(true);

    int result = QMessageBox::question(this, tr("Set Time"), tr("请在右方表中设置定时开启时间"), QMessageBox::Ok | QMessageBox::Cancel);
    if (result == QMessageBox::Ok) {
        ui->OpenCameraBtn->setEnabled(false);
        ui->CloseCameraBtn->setEnabled(false);
        ui->TakePicBtn->setEnabled(false);
        ui->SavePicBtn->setEnabled(false);
        ui->StopTimeRec->setEnabled(true);
        ui->SetTimeRec->setEnabled(false);
    } else {

        return;
    }

    //int SetTime=QInputDialog::getInt(this,"QInputDialog_Age","请输入年龄",20,0,23,1,&bOk);
    //int value = QInputDialog::getInt(this, "Input Dialog", "Enter a value:");
      QDateTime now = QDateTime::currentDateTime();
      QDateTime target = ui->dateTimeEdit->dateTime();
      int msecToTarget = now.msecsTo(target);

      if(msecToTarget<0){
         QMessageBox::warning(NULL,"Error","请重新确认时间");
      }

      ui->label_4->setText("定时时间:"+target.toString("yyyy-MM-dd HH:mm:ss"));
      EditTimer =new QTimer(this);
      EditTimer->setInterval(15000);//

      connect(EditTimer, &QTimer::timeout, this, &MainWindow::IntervalRec);

      QTimer::singleShot(msecToTarget,this,[=]{

          EditTimer->start();
          this->on_StartRecBtn_clicked();
      });


      ui->label_24->setText("已定时");
      //EditTimer->start(msecToTarget);



}
void MainWindow::on_StopTimeRec_clicked()
{

    ui->OpenCameraBtn->setEnabled(false);
    ui->CloseCameraBtn->setEnabled(true);
    ui->TakePicBtn->setEnabled(true);
    ui->SavePicBtn->setEnabled(true);
    ui->StopTimeRec->setEnabled(false);
    ui->SetTimeRec->setEnabled(true);
    EditTimer->stop();
    if(!ui->StartRecBtn->isEnabled())
    {
       this->on_EndRecBtn_clicked();
    }
    ui->label_24->setText("未定时");
}



void MainWindow::on_pushButton_clicked()
{
    if(ui->pushButton->isEnabled()==true){
        ui->pushButton->setEnabled(false);
    }else{
        ui->pushButton->setEnabled(true);
    }


    qDebug("播放中");
}
