#include "server_login.h"
#include "ui_server_login.h"
#include"QMessageBox"
#include<QSqlQuery>
Server_login::Server_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server_login)
{
    ui->setupUi(this);
    setWindowTitle("Server Login");
    ui->username->setText("admin");
    ui->password->setText("admin");
    openodbc();
    connect(ui->login,&QPushButton::clicked,this,[=](){
        QString sname=ui->username->text();
        QString spwd=ui->password->text();
//        openodbc();
        QSqlQuery query;
        qDebug()<<sname<<" "<<spwd;
        query.exec("select * from server_info where gm_name=\""+sname+"\"");
//       while(query.next()){
//           qDebug()<<query.value("gm_name").toString()
//                  <<query.value("gm_pwd").toString();
//       }
//        query.exec("select * from server_info where gm_name="+sname);
        if(!query.next())
       {
            QMessageBox box(QMessageBox::Information, "提示", "不存在该账号！");
            box.exec();
        }

        else{
            if(query.value("gm_pwd")==spwd){
//                closeodbc();
                QMessageBox box(QMessageBox::Information, "提示", "登录成功！");
                 box.setStandardButtons(QMessageBox::Ok );
                 int ret = box.exec();
                 if(ret==QMessageBox::Ok)
                    {
                     S.show();
                     this->close();
                 }
            }
            else
            {
                QMessageBox box(QMessageBox::Information, "提示", "密码错误！");
                box.exec();
//                closeodbc();
            }
        }



//        if(ui->username->text()=="admin"&&ui->password->text()=="admin"){
//            QMessageBox box(QMessageBox::Information, "提示", "登录成功！");
//             box.setStandardButtons(QMessageBox::Ok );
//             int ret = box.exec();
//             if(ret==QMessageBox::Ok)
//                {
//                 S.show();
//                 this->hide();
//             }
//        }
//        QMessageBox box(QMessageBox::Information, "提示", "登录失败");
    });
}

Server_login::~Server_login()
{
    closeodbc();
    delete ui;
}

void Server_login::openodbc()
{
    s_db = QSqlDatabase::addDatabase("QODBC");
           s_db.setHostName("127.0.0.1");
           s_db.setPort(3306);
           s_db.setDatabaseName("mimaxue"); //不是Database,记得别填错
           s_db.setUserName("root");
           s_db.setPassword("1234");
           bool ok = s_db.open();
           if (ok){
   //            QMessageBox::information(this, "infor", "link success"  );
               qDebug()<<"link success";
           }
           else {

               qDebug()<<"error ";

           }
}

void Server_login::closeodbc()
{
    s_db.close();
    s_db.removeDatabase("mimaxue");
}
