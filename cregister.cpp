#include "cregister.h"
#include "ui_cregister.h"
#include<QMessageBox>
#include<QDebug>
#include<QSqlQuery>
#include"picosha2.h"
Cregister::Cregister(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Cregister)
{
    ui->setupUi(this);
    openodbc();
    setWindowTitle("Register");

    //按钮被点击后，先通过一些条件检测，确认注册资格。
    //1.用户名长度大于等于6位
    //2.密码大于等于6位
    //3.需要确认密码需要与原密码一致。
    //满足上面三个要求后，即可注册，跳出注册成功消息对话框，注册窗口隐藏
    //存入数据库时，通过用户名和密码进行拼接进行sha-256,计算哈希值，存入哈希值。
    connect(ui->btn_rgt,&QPushButton::clicked,this,[=](){
        QString new_name=ui->new_uname->text();
        QString new_pwd=ui->new_pwd->text();
        if(new_name.size()<6)
        {
            QMessageBox box(QMessageBox::Information, "提示", "用户名需要≥6位");
            box.exec();
        }
        else if(new_pwd.length()<6){
            QMessageBox box(QMessageBox::Information, "提示", "密码需要≥6位");
            box.exec();
        }
        else {

            QSqlQuery query;

            query.exec("select * from client_info where user_name=\""+new_name+"\"");
            if(query.next()){
                QMessageBox box(QMessageBox::Information, "提示", "用户名已存在，请更改!");
                query.clear();
                box.exec();
            }
            else if(ui->cfm_pwd->text()!=new_pwd){
                QMessageBox box(QMessageBox::Information, "提示", "两次密码应该一致!");
                box.exec();
            }
            //开始存入用户唯一的哈希值，哈希值由用户名加密码进行sha-256hash
            else {
                QString Qname_pwd_sha256;
                std::string src_str;
                std::string src_str_u_p;
                std::string hash_hex_str;
                src_str=ui->new_pwd->text().toStdString();
                src_str_u_p=src_str+ui->new_uname->text().toStdString();//用户名加密码组合字符串
                std::vector<unsigned char> hash(picosha2::k_digest_size);
                picosha2::hash256(src_str_u_p.begin(),src_str_u_p.end(),hash.begin(),hash.end());
                hash_hex_str = picosha2::bytes_to_hex_string(hash.begin(),hash.end());
                Qname_pwd_sha256=QString::fromStdString(hash_hex_str);
                qDebug()<<new_name<<Qname_pwd_sha256<<QString::fromStdString(src_str);
                if(query.exec("insert into client_info(user_name,user_pwd,user_pwd_sha256) values( '"+new_name+"','"+QString::fromStdString(src_str)+"','"+Qname_pwd_sha256+"')"))//插入成功
            {
                    QMessageBox box(QMessageBox::Information, "提示", "注册成功！");
                    box.exec();
            }
                else
                {
                        QMessageBox box(QMessageBox::Information, "提示", "注册失败，请重试");
                        box.exec();
                }
            }

        }

    });


}

Cregister::~Cregister()
{
    closeodbc();
    delete ui;
}

void Cregister::openodbc()
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

void Cregister::closeodbc()
{
    db.close();
    db.removeDatabase("mimaxue");
}
