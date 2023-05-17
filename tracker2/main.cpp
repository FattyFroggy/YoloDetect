#include "mainwindow.h"

#include <QApplication>

#include"widget.h"

#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QtCore/QDebug>
#include<QStyleFactory>
#include <QtSql/QSqlDatabase>`
#include<QMessageBox>
#include <QtSql/QSqlError>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
//    w.setWindowFlag(Qt::FramelessWindowHint);
//    w.setAttribute(Qt::WA_TranslucentBackground);



    w.setWindowTitle("监控与动作识别系统");
    w.show();
    return a.exec();
}
