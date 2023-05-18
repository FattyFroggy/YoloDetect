#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "mainwindow.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

public slots:
    void set_style();

private slots:
    void on_btn_login_clicked();

    void on_btn_Register_clicked();

private:
    Ui::Widget *ui;
    bool isRegister=false;
};

#endif // WIDGET_H
