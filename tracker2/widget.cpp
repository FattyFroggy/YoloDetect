#include "widget.h"
#include "ui_widget.h"
#include <QFile>
#include <QTextStream>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include<QMessageBox>
#include <QtSql/QSqlError>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    ui->label_user_name->setScaledContents(true);   //图片自适应label大小
    ui->label_pwd->setScaledContents(true);         //图片自适应label大小

    ui->lineE_pwd->setEchoMode(QLineEdit::Password);//设置为小黑点


    connect(ui->btn_1,SIGNAL(clicked(bool)),this,SLOT(set_style()));
    connect(ui->btn_2,SIGNAL(clicked(bool)),this,SLOT(set_style()));
    connect(ui->btn_3,SIGNAL(clicked(bool)),this,SLOT(set_style()));
    connect(ui->btn_4,SIGNAL(clicked(bool)),this,SLOT(set_style()));
    ui->btn_4->click();


    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setPort(3306);
    db.setDatabaseName("Mysql"); //不是Database,记得别填错
    db.setUserName("root");
    db.setPassword("147854");
    if (db.open()) {
        qDebug() << "Database connected!";
    } else {
        qDebug() << "Failed to connect to database:" << db.lastError().text();
    }
}

/*
* 槽函数-皮肤设置
*/
QPushButton* btn;
void Widget::set_style()
{
    btn = qobject_cast<QPushButton*>(sender());//获取发射信号的对象
    QString filePath;
    if("btn_1" == btn->objectName())        //粉色
    {
        filePath = ":/res/qss/style-1.qss";
    }else if("btn_2" == btn->objectName())  //黄蓝
    {
        filePath = ":/res/qss/style-2.qss";
    }else if("btn_3" == btn->objectName())  //浅紫
    {
        filePath = ":/res/qss/style-3.qss";
    }else if("btn_4" == btn->objectName())  //青绿
    {
        filePath = ":/res/qss/style-4.qss";
    }

    /*皮肤设置*/
    QFile file(filePath);/*QSS文件所在的路径*/
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btn_login_clicked()
{
    QString UName = ui->lineE_user_name->text();
    QString UPwd = ui->lineE_pwd->text();

    if(!isRegister){
        QSqlQuery query;
        query.prepare("SELECT * FROM DUser WHERE UName = :UName AND UPwd = :UPwd");
        query.bindValue(":UName", UName);
        query.bindValue(":UPwd", UPwd);
        if (query.exec() && query.next()) {
            QMessageBox::information(NULL, "login succeed", "登录成功", QMessageBox::Yes);
            qDebug() << "Login successful!";

             MainWindow *m=new MainWindow();
             m->show();
             this->hide();

        } else {
            qDebug() << "Login failed:" << query.lastError().text();
            QMessageBox::warning(NULL,"Error"," 登录失败");
        }
    }else{
        QString i=QString("insert into DUser values ('%1','%2'); ").arg(UName).arg(UPwd);
        QString S =QString("select * from DUser where username='%1' ").arg(UName);
        QSqlQuery query;
        if(query.exec(i)){
               QMessageBox::information(NULL, "regist succeed", "注册成功", QMessageBox::Yes);
               this->on_btn_Register_clicked();}

        else if(query.exec(S)&&query.first())
               {QMessageBox::warning(NULL,"Error","重复用户名");}
        else
               {QMessageBox::warning(NULL,"Error","重复用户名");}
    }


    //this->close();




//    MainWindow *m=new MainWindow();
//    m->show();
//    this->hide();
}

void Widget::on_btn_Register_clicked()
{
    if(isRegister==false){
        ui->label_login->setText("Register");
        ui->btn_Register->setText("登录");
        ui->btn_login->setText("Register");
        isRegister=true;
         ui->lineE_user_name->clear();
         ui->lineE_pwd->clear();
    } else{
        ui->label_login->setText("Login");
        ui->btn_Register->setText("注册");
        ui->btn_login->setText("Login");
        isRegister=false;
        ui->lineE_user_name->clear();
        ui->lineE_pwd->clear();
    }




}
