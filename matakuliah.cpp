#include "matakuliah.h"
#include "ui_matakuliah.h"
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QMessageBox>
#include<QSqlQueryModel>

Matakuliah::Matakuliah(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Matakuliah)
{
    ui->setupUi(this);
    db = QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::warning(this, "Koneksi", db.lastError().text());
        return;
    }
    refresh();

}

Matakuliah::~Matakuliah()
{
    delete ui;
}

void Matakuliah::on_pushButton_clicked()
{
    //tambah
    QString kode, nama, sks, dosen;
    kode = ui->lineEdit->text();
    nama = ui->lineEdit_2->text();
    sks = ui->lineEdit_3->text();
    dosen = nip[ui->comboBox->currentIndex()];

    QSqlQuery* q = new QSqlQuery();
    q->prepare("insert into matakuliah (kode, nama_mk, sks, nip_dosen) values ('"+kode+"','"+nama+"','"+sks+"','"+dosen+"')");
    if(q->exec()){
        QMessageBox::information(this, "berhasil", "Berhasil Menambahkan!");
        refresh();
        return;
    }
    QMessageBox::warning(this, "Gagal", q->lastError().text());


}

void Matakuliah::on_pushButton_2_clicked()
{
    //update
    QString kode, nama, sks, dosen;
    kode = ui->lineEdit->text();
    nama = ui->lineEdit_2->text();
    sks = ui->lineEdit_3->text();
    dosen = nip[ui->comboBox->currentIndex()];

    QSqlQuery* q = new QSqlQuery();
    q->prepare("update matakuliah set nama_mk='"+nama+"', sks='"+sks+"', nip_dosen='"+dosen+"' where kode='"+kode+"'");
    if(q->exec()){
        QMessageBox::information(this, "berhasil", "Berhasil Diupdate!");
        refresh();
        return;
    }
    QMessageBox::warning(this, "Gagal", q->lastError().text());

}

void Matakuliah::on_pushButton_3_clicked()
{
    //delete
    QString kode;
    kode=ui->lineEdit->text();
    QSqlQuery* q = new QSqlQuery();
    q->prepare("delete from matakuliah where kode='"+kode+"'");
    if(q->exec()){
        QMessageBox::information(this, "berhasil", "Berhasil Dihapus!!");
        refresh();
        return;
    }
    QMessageBox::warning(this, "Gagal", q->lastError().text());
}

void Matakuliah::on_pushButton_4_clicked()
{
    refresh();
}

void Matakuliah::refresh(){
    QSqlQuery* q = new QSqlQuery();
    q->prepare("select kode as Kode, nama_mk as Matakuliah, sks as SKS, dosen.nama_dosen as Pengampu from matakuliah join dosen on (matakuliah.nip_dosen=dosen.nip)");
    q->exec();
//    QMessageBox::information(0, "", q->lastError().text());
    QSqlQueryModel* m = new QSqlQueryModel();
    m->setQuery(*q);
    ui->tableView->setModel(m);

    dos[0] = "=Pilih Dosen=";
    in = 1;
    QSqlQuery* qu = new QSqlQuery();
    qu->prepare("select * from dosen");
    qu->exec();
    while(qu->next()){
        dos[in]=qu->value("nama_dosen").toString();
        nip[in]=qu->value("nip").toString();
        in++;
    }

    ui->comboBox->clear();
    for(int i=0; i<in; i++){
        ui->comboBox->addItem(dos[i]);
    }
    ui->comboBox->setCurrentIndex(0);
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->pushButton_2->setVisible(false);
    ui->pushButton_3->setVisible(false);
    ui->pushButton->setVisible(true);
    ui->lineEdit->setDisabled(false);

}

void Matakuliah::on_tableView_clicked(const QModelIndex &index)
{
    QString kode, nama, sks, dosen;
    kode = ui->tableView->model()->index(index.row(), 0).data().toString();
    nama = ui->tableView->model()->index(index.row(), 1).data().toString();
    sks = ui->tableView->model()->index(index.row(), 2).data().toString();
    dosen = ui->tableView->model()->index(index.row(), 3).data().toString();
    ui->lineEdit->setText(kode);
    ui->lineEdit_2->setText(nama);
    ui->lineEdit_3->setText(sks);
    int select = 0;
    for(int i=0; i<in; i++){
        if(dos[i]==dosen){
            select = i;
        }
    }
    ui->comboBox->setCurrentIndex(select);
    ui->pushButton->setVisible(false);
    ui->pushButton_2->setVisible(true);
    ui->pushButton_3->setVisible(true);
    ui->lineEdit->setDisabled(true);

}
