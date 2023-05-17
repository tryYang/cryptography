#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include<QTcpServer>
#include<QTcpSocket>
#include<QTimer>
#include<QFile>

namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = nullptr);
    ~Server();
    void sendfiledata();
signals:
    void send_file();
private:
    Ui::Server *ui;
    QTcpServer *server;
    QTcpSocket *conn;
    QTcpSocket *m_fileSocket;
    QFile file;

    QString fileName;       // 文件名字
    qint64  fileSize;       // 文件大小
    qint64 sendSize;        // 已经发送的文件的大小
};

#endif // SERVER_H
