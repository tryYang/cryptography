#include "widget.h"

#include"QPushButton"
#include <QApplication>
#include<QSqlDatabase>
#include<QDebug>
#include<QSqlError>
#include<QMessageBox>
#include<QSqlQuery>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("主界面");
//    QSqlDatabase db=QSqlDatabase::addDatabase("QODBC");
//    db.setHostName("127.0.0.1");
//    db.setPort(3306);
//    db.setDatabaseName("mimaxue"); //不是Database,记得别填错
//    db.setUserName("root");
//    db.setPassword("1234");
//    bool ok = db.open();
//    if (ok){
//      QMessageBox::information(this, "infor", "link success"  );
//        qDebug()<<"link success";
//    }
//    else {

//        qDebug()<<"error ";

//    }
//    QSqlQuery query;
//    query.exec("select user_pwd_sha256 from client_info where user_name=\'12312345\'");
//    query.next();
//    qDebug()<<query.value("user_pwd_sha256").toString();






//    qDebug()<<QSqlDatabase::drivers();
    w.show();



    return a.exec();
}
