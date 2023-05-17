#include "client_login.h"
#include "ui_client_login.h"
#include"QMessageBox"
#include"QDebug"
#include "picosha2.h"
#include<QSqlQuery>
Client_login::Client_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client_login)
{
    ui->setupUi(this);
    setWindowTitle("Client Login");
    ui->username->setText("666");
    ui->password->setText("666");
    openodbc();

//    std::string src_str=ui->password->text().toStdString();
//    std::string hash_hex_str;
//    src_str+=ui->username->text().toStdString();
//    std::vector<unsigned char> hash(picosha2::k_digest_size);
//    picosha2::hash256(src_str.begin(),src_str.end(),hash.begin(),hash.end());
//    hash_hex_str = picosha2::bytes_to_hex_string(hash.begin(),hash.end());
//    qstr=QString::fromStdString(hash_hex_str);
//    qDebug()<<qstr;
    connect(ui->btn_register,&QPushButton::clicked,this,[=](){
        R.show();
        this->close();
    });
    connect(ui->login,&QPushButton::clicked,this,[=](){
        QString cname=ui->username->text();
        QString cpwd=ui->password->text();
//        openodbc();
        QString qstr;
        std::string src_str=cpwd.toStdString();
        std::string hash_hex_str;
        src_str+=cname.toStdString();
        std::vector<unsigned char> hash(picosha2::k_digest_size);
        picosha2::hash256(src_str.begin(),src_str.end(),hash.begin(),hash.end());
        hash_hex_str = picosha2::bytes_to_hex_string(hash.begin(),hash.end());
        qstr=QString::fromStdString(hash_hex_str);









        QSqlQuery query;
        query.exec("select * from client_info where user_name=\""+cname+"\"");
//        while (query.next()) {
//            qDebug()<<query.value("user_name").toString()
//                 <<query.value("user_pwd_sha256").toString();
//        }
        if(!query.next()){
             qDebug()<<"用户名不存在";
//             closeodbc();
        }

        else{
            qDebug()<<qstr;
            if(query.value("user_pwd_sha256")==qstr){
                closeodbc();
                QMessageBox box(QMessageBox::Information, "提示", "登录成功！");
                 box.setStandardButtons(QMessageBox::Ok );
                 int ret = box.exec();
                 if(ret==QMessageBox::Ok)
                    {
                     C.show();
                     this->close();
                 }
            }
            else
            {
                qDebug()<<"密码错误";
//                closeodbc();
            }
        }

//        closeodbc();
//        if(cname=="666"&&cpwd=="666"){
//            QMessageBox box(QMessageBox::Information, "提示", "登录成功！");
//             box.setStandardButtons(QMessageBox::Ok );
//             int ret = box.exec();
//             if(ret==QMessageBox::Ok)
//                {
//                 C.show();
//                 this->hide();
//             }
//        }
//        else{
//            ui->state->setText("Input error, please re-enter");

//        }

    });
}

Client_login::~Client_login()
{
    closeodbc();
    delete ui;
}

void Client_login::openodbc()
{
    db = QSqlDatabase::addDatabase("QODBC");
           db.setHostName("127.0.0.1");
           db.setPort(3306);
           db.setDatabaseName("mimaxue"); //不是Database,记得别填错
           db.setUserName("root");
           db.setPassword("1234");
           bool ok = db.open();
           if (ok){
   //            QMessageBox::information(this, "infor", "link success"  );
               qDebug()<<"link success";
           }
           else {

               qDebug()<<"error ";

           }
}

void Client_login::closeodbc()
{
    db.close();
    db.removeDatabase("mimaxue");
}
