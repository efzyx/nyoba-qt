#include "datamahasiswa.h"
#include "ui_datamahasiswa.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QSqlError>
#include<QModelIndex>

DataMahasiswa::DataMahasiswa(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataMahasiswa)
{
    ui->setupUi(this);
    db = QSqlDatabase::database();
    QSqlQuery* q = new QSqlQuery();
    q->prepare("select nim as NIM, nama as Nama, jk as JK, smu as Asal from mahasiswa");
    q->exec();
    QSqlQueryModel* m = new QSqlQueryModel();
    m->setQuery(*q);
    ui->tableView->setModel(m);
    ui->labelGambar->setScaledContents(true);

}

DataMahasiswa::~DataMahasiswa()
{
    delete ui;
}

void DataMahasiswa::on_pushButton_pilih_clicked()
{

//        QMessageBox::information(0,"",name);
        file = QFileDialog::getOpenFileName(this, tr("Pilih Gambar"), "C:\\Users\\fauzi\\Pictures\\", "All Files (*.*);; JPG (*.jpg)");
//        ui->menuInput_Data->menuAction()->setVisible(false);
        QPixmap pix(file);
        ui->labelGambar->setPixmap(pix);

}

void DataMahasiswa::on_pushButton_clicked()
{
  QString nim = ui->lineEdit_nim->text();
  QString nama = ui->lineEdit_nama->text();
  QString jk = ui->radioButton_lak->isChecked() ? "Laki-laki" : "Perempuan";
  QString smu = ui->lineEdit_asal->text();

  QSqlQuery* q = new QSqlQuery();
  q->prepare("insert into mahasiswa (nim, nama, jk, smu, photo) values ('"+nim+"','"+nama+"','"+jk+"','"+smu+"','"+file+"')");
  if(q->exec()){
      QMessageBox::information(this, "Berhasil", "Berhasil ditambahkan");
      refresh();
  }
}


void DataMahasiswa::on_pushButton_delete_clicked()
{
        QString nim = ui->lineEdit_nim->text();
        QSqlQuery* q = new QSqlQuery();
        q->prepare("delete from mahasiswa where nim ='"+nim+"'");
        if(q->exec()){
            QMessageBox::information(this, "Berhasil", "Berhasil dihapus!");
            refresh();
        }else{
            QMessageBox::warning(this, "Error", q->lastError().text());
        }

}

void DataMahasiswa::refresh(){
    QSqlQuery* q = new QSqlQuery();
    q->prepare("select nim as NIM, nama as Nama, jk as JK, smu as Asal from mahasiswa");
    q->exec();
    QSqlQueryModel* m = new QSqlQueryModel();
    m->setQuery(*q);
    ui->tableView->setModel(m);
    clear();

}

void DataMahasiswa::on_tableView_clicked(const QModelIndex &index)
{
    QString nim, nama, photo, jk, smu;
    nim = ui->tableView->model()->index(index.row(), 0).data().toString();
    nama = ui->tableView->model()->index(index.row(), 1).data().toString();
    QSqlQuery* q = new QSqlQuery();
    q->prepare("select * from mahasiswa where nim='"+nim+"'");
    q->exec();
    q->first();
    file = q->value("photo").toString();
    jk = ui->tableView->model()->index(index.row(), 2).data().toString();
    smu = ui->tableView->model()->index(index.row(), 3).data().toString();
    ui->lineEdit_nim->setText(nim);
    ui->lineEdit_nama->setText(nama);
    ui->radioButton_lak->setChecked(jk == "Laki-laki" ? true : false);
    ui->radioButton_Per->setChecked(jk == "Perempuan" ? true : false);
    ui->lineEdit_asal->setText(smu);
    QPixmap pix(file);
    ui->labelGambar->setPixmap(pix);
    ui->pushButton->setVisible(false);
    ui->pushButton_delete->setVisible(true);
    ui->pushButton_update->setVisible(true);
    ui->lineEdit_nim->setDisabled(true);

}

void DataMahasiswa::clear(){
    ui->labelGambar->clear();
    ui->lineEdit_asal->clear();
    ui->lineEdit_nama->clear();
    ui->lineEdit_nim->clear();
    ui->radioButton_lak->setAutoExclusive(false);
    ui->radioButton_lak->setChecked(false);
    ui->radioButton_lak->setAutoExclusive(true);
    ui->radioButton_Per->setAutoExclusive(false);
    ui->radioButton_Per->setChecked(false);
    ui->radioButton_Per->setAutoExclusive(true);
    ui->pushButton->setVisible(true);
    ui->pushButton_delete->setVisible(false);
    ui->pushButton_update->setVisible(false);
    ui->lineEdit_nim->setDisabled(false);
}

void DataMahasiswa::on_pushButton_update_clicked()
{
    QString nim = ui->lineEdit_nim->text();
    QString nama = ui->lineEdit_nama->text();
    QString jk = ui->radioButton_lak->isChecked() ? "Laki-laki" : "Perempuan";
    QString smu = ui->lineEdit_asal->text();

    QSqlQuery* q = new QSqlQuery();
    q->prepare("update mahasiswa set nama='"+nama+"', jk='"+jk+"', smu='"+smu+"', photo='"+file+"' where nim ='"+nim+"'");
    if(q->exec()){
        QMessageBox::information(this, "Berhasil", "Berhasil diupdate!");
        refresh();
    }else{
        QMessageBox::warning(this, "Error", q->lastError().text());
    }


}

void DataMahasiswa::on_pushButton_Clear_clicked()
{
    refresh();
}
