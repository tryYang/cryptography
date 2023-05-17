#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include<QTcpSocket>
#include<server.h>
namespace Ui {
class Client;
}

class Client : public QWidget
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = nullptr);
    ~Client();
   // void change();
    //Server s;
//private slots:
//    void slot_change();
private:
    Ui::Client *ui;
    QTcpSocket *client;
    bool isfile=false;
};

#endif // CLIENT_H
