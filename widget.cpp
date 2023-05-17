#include "widget.h"
#include "ui_widget.h"
#include"QPushButton"
#include"client_login.h"
#include"server_login.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(ui->client_login,&QPushButton::clicked,this,[=](){

        C.show();
    });
    connect(ui->server_login,&QPushButton::clicked,this,[=](){

        S.show();
    });
}

Widget::~Widget()
{
    delete ui;
}

