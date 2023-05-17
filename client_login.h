#ifndef CLIENT_LOGIN_H
#define CLIENT_LOGIN_H

#include <QWidget>
#include"client.h"
#include<QSqlDatabase>
#include"cregister.h"
namespace Ui {
class Client_login;
}

class Client_login : public QWidget
{
    Q_OBJECT

public:
    explicit Client_login(QWidget *parent = nullptr);
    ~Client_login();
      Client C;
      Cregister R;

      void openodbc();
      void closeodbc();
private:
    Ui::Client_login *ui;

    QSqlDatabase db;
};

#endif // CLIENT_LOGIN_H
