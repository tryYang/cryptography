#include "server.h"
#include "ui_server.h"
#include"AES.h"
#include"Base64.h"
#include<QFileDialog>
#include<QDebug>
using namespace std;
const char g_key[17] = "asdfwetyhjuytrfd";
const char g_iv[17] = "gfdertfghjkuyrtg";//ECB MODE不需要关心chain，可以填空
string EncryptionAES(const string& strSrc) //AES加密
{
    size_t length = strSrc.length();
    int block_num = length / BLOCK_SIZE + 1;
    //明文
    char* szDataIn = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataIn, 0x00, block_num * BLOCK_SIZE + 1);
    strcpy(szDataIn, strSrc.c_str());

    //进行PKCS7Padding填充。
    int k = length % BLOCK_SIZE;
    int j = length / BLOCK_SIZE;
    int padding = BLOCK_SIZE - k;
    for (int i = 0; i < padding; i++)
    {
        szDataIn[j * BLOCK_SIZE + k + i] = padding;
    }
    szDataIn[block_num * BLOCK_SIZE] = '\0';

    //加密后的密文
    char* szDataOut = new char[block_num * BLOCK_SIZE + 1];
    memset(szDataOut, 0, block_num * BLOCK_SIZE + 1);

    //进行进行AES的CBC模式加密
    AES aes;
    aes.MakeKey(g_key, g_iv, 16, 16);
    aes.Encrypt(szDataIn, szDataOut, block_num * BLOCK_SIZE, AES::CBC);
    string str = base64_encode((unsigned char*)szDataOut,
        block_num * BLOCK_SIZE);
    delete[] szDataIn;
    delete[] szDataOut;
    return str;
}
Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    setWindowTitle("Server");
    ui->stop_listen->setEnabled(false);
    ui->S_record->setReadOnly(true);
    ui->msg_send->setEnabled(false);
    QTimer * msg_timer = new QTimer(this);
    QTimer * file_timer = new QTimer(this);
    ui->select_f->setEnabled(false);
    ui->send_f->setEnabled(false);
               // 传输的文件对象
    QString filename;
    //开始监听
    connect(ui->start_listen,&QPushButton::clicked,this,[=](){
       server =new QTcpServer(this);//实例化
       server->listen(QHostAddress("127.0.0.1"),666);
       qDebug()<<"主机地址：127.0.0.1 监听端口：666";
       ui->S_record->append("主机地址：127.0.0.1 监听端口：666");
       ui->S_record->append("开始监听.........................");
       ui->start_listen->setEnabled(false);
       ui->stop_listen->setEnabled(true);
       //定时器，查看是否断开连接



       connect(server,&QTcpServer::newConnection,this,[=](){
           ui->stop_listen->setEnabled(false);
           ui->select_f->setEnabled(true);
           // 获取连接好的套接字
            msg_timer->start(1000);
           conn = server->nextPendingConnection();
           // 获取对方的ip好端口
            QString ip = conn->peerAddress().toString();
            quint16 port = conn->peerPort();
            QString str = QString("[%1, %2] 成功连接").arg(ip).arg(port);
            ui->S_record->append(str);
            ui->msg_send->setEnabled(true);
            connect(conn,&QTcpSocket::readyRead,this,[=](){
               //接收数据
                QByteArray array =conn->readAll();
                ui->S_record->append("客户端:"+array);

            });

       });
    });
    //定时器查看是否断开连接
    connect(msg_timer,&QTimer::timeout,this,[=](){
            if(conn->state()==QAbstractSocket::UnconnectedState)
            {
                msg_timer->stop();

                    server->close();

                    conn->disconnectFromHost();
                    conn->close();
                    ui->S_record->append("link stop");
                    ui->msg_send->setEnabled(false);
                    ui->start_listen->setEnabled(true);

            }
        });

    //消息发送
    connect(ui->msg_send,&QPushButton::clicked,this,[=](){

        if(ui->s_input->text()=="")
            qDebug()<<"输入不可为空";
        else if(conn->state()==0){
            qDebug()<<"未建立连接";
        }
        else if(conn->state()==QAbstractSocket::ConnectedState){
            QString qstr=ui->s_input->text();
            const string s1=qstr.toStdString();
            string s2;
             ui->S_record->append("我："+qstr);
            s2=EncryptionAES(s1);
            qstr = QString::fromStdString(s2);
            conn->write(qstr.toUtf8().data());

            ui->s_input->clear();
        }

    });
    //停止监听
    connect(ui->stop_listen,&QPushButton::clicked,this,[=](){
        server->close();//停止监听
        ui->start_listen->setEnabled(true);
        ui->stop_listen->setEnabled(false);
//        ui->S_record->clear();
        ui->S_record->append("停止监听");
    });
    //断开连接信号
    //选择文件按钮

    connect(ui->select_f,&QPushButton::clicked,this,[=](){



       QString filePath = QFileDialog::getOpenFileName(this, "open", "../");
       if(false == filePath.isEmpty()){   // 如果选择的文件有效
               // 获取信息前应当先清空
               fileName.clear();
               fileSize = 0;

               // 获取文件信息（以只读的方式打开）
               QFileInfo info(filePath);
               fileName = info.fileName();

               fileSize = info.size();

               sendSize = 0;   // 发送文件的大小

               // 以只读的方式打开文件
               file.setFileName(filePath);        // 指定文件名字
               bool isOk = file.open(QIODevice::ReadOnly); // 只读方式打开文件

               if(false == isOk){
                   ui->S_record->append("只读方式打开文件失败 70");
               }
               else{
                   ui->S_record->append(QString("打开的文件是: %1").arg(filePath));
                   ui->send_f->setEnabled(true);

               }
       }else{
               qDebug() << "选择文件路径出错 74";
           }


    });
    connect(file_timer,&QTimer::timeout,this,[=](){
        file_timer->stop();
        sendfiledata();
    });
    //发送文件按钮
    connect(ui->send_f,&QPushButton::clicked,this,[=](){
        ui->send_f->setEnabled(false);
        QString buf = QString("head#%1#%2").arg(fileName).arg(fileSize);
        //发送信号
       // emit send_file();
            //先发头
            qint64 len = conn->write(buf.toUtf8().data());
            conn->waitForBytesWritten();
            ui->S_record->append("已经在发送文件！！");

                if(len>0)//如果头部信息发送成功，开始发送文件数据
                {
                    //10毫秒后在发送文件数据
                    //启动定时器，定时器内容发送文件数据
                    //防止tcp黏包问题
                    file_timer->start(1000);//为了延时
                }
                else
                {
                    file.close();
                }



    });

}

Server::~Server()
{
    delete ui;
}

void Server::sendfiledata()
{
    //发送文件信息
        //发送文件
        qint64 len = 0 ;

        do{
            //每次发送2kb大小的数据，如果剩余的数据不足2kb，就发送剩余数据的大小
            char buf[2*1024]= {0};

            len = 0;
            len=file.read(  buf , sizeof(buf) );        //读数据
            len=conn->write(buf , len);            //发数据

            //发送数据需要累积
            sendSize += len;

        }while(len>0 );
        //文件数据发送完毕
        if (fileSize == sendSize)
            {
            //QMessageBox::information(this,"ok","文件发送完毕")；
            ui->S_record->append("发送成功");

            file.close();//关闭文件
            //关闭客户端

        }


}
