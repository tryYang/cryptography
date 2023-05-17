#ifndef CREGISTER_H
#define CREGISTER_H

#include <QWidget>
#include<QSqlDatabase>
namespace Ui {
class Cregister;
}

class Cregister : public QWidget
{
    Q_OBJECT

public:
    explicit Cregister(QWidget *parent = nullptr);
    ~Cregister();
    void openodbc();
    void closeodbc();
private:
    Ui::Cregister *ui;
    QSqlDatabase db;
};

#endif // CREGISTER_H
