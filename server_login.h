#ifndef SERVER_LOGIN_H
#define SERVER_LOGIN_H

#include <QWidget>
#include<server.h>
#include<QSqlDatabase>
namespace Ui {
class Server_login;
}

class Server_login : public QWidget
{
    Q_OBJECT

public:
    explicit Server_login(QWidget *parent = nullptr);
    ~Server_login();
    Server S;
    void openodbc();
    void closeodbc();
private:
    Ui::Server_login *ui;
    QSqlDatabase s_db;
};

#endif // SERVER_LOGIN_H
