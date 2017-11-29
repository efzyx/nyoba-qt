#include "datauser.h"
#include "ui_datauser.h"
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QMessageBox>
#include<QSqlQueryModel>

DataUser::DataUser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataUser)
{
    ui->setupUi(this);
    db = QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::warning(this, "Koneksi gagal", db.lastError().text());
        return;
    }
    refresh();
}

DataUser::~DataUser()
{
    delete ui;
}

void DataUser::on_pushButton_clicked()
{
  QString username = ui->lineEdit->text();
  QString pass = ui->lineEdit_2->text();
  QString role = ui->comboBox->currentText();
  QString roleDb = role == "admin" ? "1" : role == "dosen" ? "2" : role == "mahasiswa" ? "3" : "0";

  QSqlQuery *q = new QSqlQuery();
  q->prepare("insert into user (username, password, group_akses) values ('"+username+"','"+pass+"','"+roleDb+"')");
  if(q->exec()){
      QMessageBox::information(this, "Berhasil", "Beerhasil menambahkan!");
      refresh();
      return;
  }
  QMessageBox::warning(this, "Gagal", q->lastError().text());

}

void DataUser::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    QString pass = ui->lineEdit_2->text();
    QString role = ui->comboBox->currentText();
    QString roleDb = role == "admin" ? "1" : role == "dosen" ? "2" : role == "mahasiswa" ? "3" : "0";

    QSqlQuery *q = new QSqlQuery();
    q->prepare("update user set password ='"+pass+"', group_akses='"+roleDb+"' where username='"+username+"'");
    if(q->exec()){
        QMessageBox::information(this, "Berhasil", "Beerhasil update!");
        refresh();
        return;
    }
    QMessageBox::warning(this, "Gagal", q->lastError().text());

}

void DataUser::on_pushButton_3_clicked()
{
    QString username = ui->lineEdit->text();

    QSqlQuery *q = new QSqlQuery();
    q->prepare("delete from user where username='"+username+"'");
    if(q->exec()){
        QMessageBox::information(this, "Berhasil", "Beerhasil delete!");
        refresh();
        return;
    }
    QMessageBox::warning(this, "Gagal", q->lastError().text());
}

void DataUser::on_pushButton_4_clicked()
{
    refresh();
}

void DataUser::refresh(){
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();

    QSqlQuery *q = new QSqlQuery();
    q->prepare("select username as Username, case group_akses when 1 then 'admin' when 2 then 'dosen' when 3 then 'mahasiswa' else 'Belum Diset' end as Role from user");
    q->exec();
    QSqlQueryModel *m = new QSqlQueryModel();
    m->setQuery(*q);
    ui->tableView->setModel(m);
    ui->comboBox->clear();
    ui->comboBox->addItem("+ Pilih Role +");
    ui->comboBox->addItem("admin");
    ui->comboBox->addItem("dosen");
    ui->comboBox->addItem("mahasiswa");
    ui->comboBox->setCurrentIndex(0);
    ui->pushButton->setVisible(true);
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->lineEdit->setDisabled(false);
}

void DataUser::on_tableView_clicked(const QModelIndex &index)
{
    QString username;
    username = ui->tableView->model()->index(index.row(), 0).data().toString();
    QSqlQuery *q = new QSqlQuery();
    q->prepare("select * from user where username='"+username+"'");
    q->exec();
    q->first();
    ui->lineEdit->setText(username);
    ui->comboBox->setCurrentIndex(q->value("group_akses").toInt());
    ui->lineEdit_2->setText(q->value("password").toString());
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->lineEdit->setDisabled(true);
}
