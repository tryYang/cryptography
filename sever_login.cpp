#include "sever_login.h"
#include "ui_sever_login.h"

Sever_login::Sever_login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sever_login)
{
    ui->setupUi(this);
}

Sever_login::~Sever_login()
{
    delete ui;
}
