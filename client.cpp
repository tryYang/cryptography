#include "client.h"
#include "ui_client.h"
#include"AES.h"
#include"Base64.h"
#include<iostream>
#include<QHostAddress>
using namespace std;
const char g_key[17] = "asdfwetyhjuytrfd";
const char g_iv[17] = "gfdertfghjkuyrtg";//ECB MODE不需要关心chain，可以填空
string DecryptionAES(const string& strSrc) //AES解密
{
    string strData = base64_decode(strSrc);
    size_t length = strData.length();
    //密文
    char* szDataIn = new char[length + 1];
    memcpy(szDataIn, strData.c_str(), length + 1);
    //明文
    char* szDataOut = new char[length + 1];
    memcpy(szDataOut, strData.c_str(), length + 1);

    //进行AES的CBC模式解密
    AES aes;
    aes.MakeKey(g_key, g_iv, 16, 16);
    aes.Decrypt(szDataIn, szDataOut, length, AES::CBC);

    //去PKCS7Padding填充
    if (0x00 < szDataOut[length - 1] <= 0x16)
    {
        int tmp = szDataOut[length - 1];
        for (int i = length - 1; i >= length - tmp; i--)
        {
            if (szDataOut[i] != tmp)
            {
                memset(szDataOut, 0, length);
                cout << "去填充失败！解密出错！！" << endl;
                break;
            }
            else
                szDataOut[i] = 0;
        }
    }
    string strDest(szDataOut);
    delete[] szDataIn;
    delete[] szDataOut;
    return strDest;
}
Client::Client(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);
    setWindowTitle("Client");
    client=new QTcpSocket(this);
    ui->link_stop->setEnabled(false);
    ui->record->setReadOnly(true);
    ui->msg_send->setEnabled(false);
    ui->link_stop->setEnabled(false);
    ui->s_ip->setText("127.0.0.1");
    ui->s_port->setText("666");
        //尝试连接
        connect(ui->link_start,&QPushButton::clicked,this,[=](){
        client->connectToHost(QHostAddress(ui->s_ip->text()),ui->s_port->text().toInt());
        connect(client,&QAbstractSocket::connected,this,[=](){
            ui->record->append("连接成功");
            ui->link_start->setEnabled(false);
            ui->msg_send->setEnabled(true);
            ui->link_stop->setEnabled(true);
        });

        //判断是否连接成功
//        if(ui->s_ip->text()=="127.0.0.1"&&ui->s_port->text().toInt()==666){
//                ui->record->append("连接成功");
//                ui->link_start->setEnabled(false);
//                ui->msg_send->setEnabled(true);
//                ui->link_stop->setEnabled(true);
//          }

        //读取服务端的消息数据
        connect(client,&QTcpSocket::readyRead,this,[=](){

            QByteArray array =client->readAll();
            string s1=array.toStdString();
            QString qstr=QString::fromStdString(DecryptionAES(s1));
            ui->record->append("服务器：密文:"+array);
            ui->record->append("        明文:"+qstr);

        });
        //发送消息
        connect(ui->msg_send,&QPushButton::clicked,this,[=](){
            if(ui->c_input->text()=="")
                qDebug()<<"输入不可为空";
            else if(client->state()==0){
                qDebug()<<"未建立连接";
            }
            else if(client->state()==QAbstractSocket::ConnectedState){
                client->write(ui->c_input->text().toUtf8().data());
                ui->record->append("我："+ui->c_input->text());
                ui->c_input->clear();
            }



        });
//        connect(&s,SIGNAL(send_file()),this,SLOT(slot_change()));
        //主动断开连接
        connect(ui->link_stop,&QPushButton::clicked,this,[=](){
             //主动和对方断开连接
            if(client->state()!=0){
                client->disconnectFromHost();
                client->close();
                ui->record->append(" stop linking");
                ui->link_start->setEnabled(true);
                ui->link_stop->setEnabled(false);
                ui->msg_send->setEnabled(false);
            }

        });

    });
}

Client::~Client()
{
    delete ui;
}

//void Client::change()
//{
//    this->isfile=!this->isfile;

//}

//void Client::slot_change()
//{
//    this->change();
//}
