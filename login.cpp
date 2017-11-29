#include "login.h"
#include "ui_login.h"
#include "mysqlconnect.h"
#include<QMessageBox>
#include<QSqlError>
#include <QStyle>
#include <QDesktopWidget>
#include <QSqlQuery>
#include <mainwindow.h>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    db = MysqlConnect::getConnect("localhost", "root", "", "cobaqt");
    if(!db.open()){
        QMessageBox::warning(this, "Login", db.lastError().text());
        return;
    }
    ui->setupUi(this);

    window()->setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            window()->size(),
            qApp->desktop()->availableGeometry()
        )
    );
    QIcon icon = windowIcon();

    Qt::WindowFlags flags = windowFlags();

    Qt::WindowFlags helpFlag =
    Qt::WindowContextHelpButtonHint;

    flags = flags & (~helpFlag);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | flags);

}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    QString username = ui->lineEdit_Username->text();
    QString password = ui->lineEdit_Password->text();

    if(!db.open()){
        QMessageBox::warning(this, "Koneksi Gagal", db.lastError().text());
        return;
    }

    QSqlQuery query;
    query.prepare("select * from user where username='"+username+"'");
    query.exec();
    if(query.size() > 0){
        query.first();
        if(query.value("password").toString() == password){
            query.finish();
            ui->lineEdit_Username->clear();
            ui->lineEdit_Password->clear();
            query.clear();
            db.close();
            hide();
//            mainWindow = new MainWindow(this);
            mainWindow.setData(username);
            mainWindow.show();
        }else{
            QMessageBox::warning(this, "Peringatan", "Password tidak cocok untuk username ini");
            ui->lineEdit_Password->clear();
        }
    }else{
        QMessageBox::warning(this, "Peringatan", "Username tidak ditemukan");
        ui->lineEdit_Username->clear();
        ui->lineEdit_Password->clear();
    }
}
