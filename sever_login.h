#ifndef SEVER_LOGIN_H
#define SEVER_LOGIN_H

#include <QWidget>

namespace Ui {
class Sever_login;
}

class Sever_login : public QWidget
{
    Q_OBJECT

public:
    explicit Sever_login(QWidget *parent = nullptr);
    ~Sever_login();

private:
    Ui::Sever_login *ui;
};

#endif // SEVER_LOGIN_H
