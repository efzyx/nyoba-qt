#include "datadosen.h"
#include "ui_datadosen.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlQueryModel>
#include<QSqlError>
#include<QModelIndex>
#include<QStringList>
#include<QAbstractItemModel>

DataDosen::DataDosen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataDosen)
{
    ui->setupUi(this);


    db = QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::warning(this, "Koneksi", db.lastError().text());
        return;
    }
    QSqlQuery* q = new QSqlQuery();
    q->prepare("select nip as NIP, nama_dosen as Nama, prodi as Prodi from dosen");
    q->exec();
    QSqlQueryModel* m = new QSqlQueryModel();
    m->setQuery(*q);
    ui->tableView->setModel(m);
    q->clear();
    refresh();

}

DataDosen::~DataDosen()
{
    delete ui;
}

void DataDosen::on_pushButton_Close_clicked()
{
    accept();
    QApplication::activeModalWidget();
    mw.raise();
}


void DataDosen::on_pushButton_update_clicked()
{
    QString nip = ui->lineEdit_nip->text();
    QString nama = ui->lineEdit_nama->text();
    QString prodi = ui->lineEdit_prodi->text();

    if(!db.open()){
        return;
    }
    QSqlQuery* q = new QSqlQuery();
    q->prepare("update dosen set nama_dosen='"+nama+"',prodi='"+prodi+"' where nip='"+nip+"'");
    if(q->exec()){
        QMessageBox::information(this, "berhasil", "Berhasil diupdate!");
        refresh();
        clear();
    }else{
        QMessageBox::warning(this, "Error", "Gagal!");
    }
}

void DataDosen::on_pushButton_tambah_clicked()
{
    QString nip = ui->lineEdit_nip->text();
    QString nama = ui->lineEdit_nama->text();
    QString prodi = ui->lineEdit_prodi->text();

    if(!db.open()){
        return;
    }

    QSqlQuery* q = new QSqlQuery();
    q->prepare("insert into dosen (nip, nama_dosen, prodi) values ('"+nip+"','"+nama+"','"+prodi+"')");
    if(q->exec()){
        QMessageBox::information(this, "berhasil", "Berhasil ditambahkan");
        refresh();
        clear();
    }else{
        QMessageBox::warning(this, "Error", "Data sudah ada!");
    }
}


void DataDosen::on_pushButton_delete_clicked()
{
    QString nip = ui->lineEdit_nip->text();
    QSqlQuery* q = new QSqlQuery();
    q->prepare("delete from dosen where nip='"+nip+"'");
    if(q->exec()){
        QMessageBox::information(this, "Delete", "Berhasil dihapus");
        refresh();
        clear();
    }else{
        QMessageBox::warning(this, "Error", q->lastError().text());
    }

}

void DataDosen::refresh(){
    QSqlQuery* q = new QSqlQuery();
    q->prepare("select nip as NIP, nama_dosen as Nama, prodi as Prodi from dosen");
    q->exec();
    m = new QSqlQueryModel();
    m->setQuery(*q);
    ui->tableView->setModel(m);
    q->clear();
    ui->pushButton_tambah->setVisible(true);
    ui->pushButton_update->setVisible(false);
    ui->pushButton_delete->setVisible(false);
    clear();
}

void DataDosen::on_tableView_clicked(const QModelIndex &index)
{
        QString nip, nama, prodi;
        nip = ui->tableView->model()->index(index.row(), 0).data().toString();
        nama = ui->tableView->model()->index(index.row(), 1).data().toString();
        prodi = ui->tableView->model()->index(index.row(), 2).data().toString();
        ui->lineEdit_nip->setText(nip);
        ui->lineEdit_nama->setText(nama);
        ui->lineEdit_prodi->setText(prodi);
        ui->pushButton_tambah->setVisible(false);
        ui->pushButton_update->setVisible(true);
        ui->pushButton_delete->setVisible(true);
        ui->lineEdit_nip->setDisabled(true);
}

void DataDosen::clear(){
    ui->lineEdit_nama->clear();
    ui->lineEdit_nip->clear();
    ui->lineEdit_prodi->clear();
    ui->lineEdit_nip->setDisabled(false);
}

void DataDosen::on_pushButton_clear_clicked()
{
    refresh();
}

void DataDosen::on_DataDosen_accepted()
{

}
