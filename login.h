#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include<QSqlDatabase>
#include<mysqlconnect.h>
#include<mainwindow.h>
#include<QLineEdit>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = 0);
    ~Login();
    MainWindow mainWindow;
    bool closeCon(){
        MysqlConnect::closeConnection(db);
        return true;
    }

private slots:
    void on_pushButton_clicked();

private:
    Ui::Login *ui;
    QSqlDatabase db;

};

#endif // LOGIN_H
