#include "videowriterworker.h"
#include "ui_videowriterworker.h"

VideoWriterWorker::VideoWriterWorker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VideoWriterWorker)
{
    ui->setupUi(this);
}

VideoWriterWorker::~VideoWriterWorker()
{
    delete ui;
}
