#include "nilai.h"
#include "ui_nilai.h"
#include<QSqlDatabase>
#include<QSqlError>
#include<QSqlQuery>
#include<QMessageBox>
#include<QSqlQueryModel>

Nilai::Nilai(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nilai)
{
    ui->setupUi(this);
    db = QSqlDatabase::database();
    refresh();
}

Nilai::~Nilai()
{
    delete ui;
}

void Nilai::on_pushButton_Tambah_clicked()
{
   QString mahasiswa = nim[ui->comboBox->currentIndex()];
   QString matakuliah = kod[ui->comboBox_2->currentIndex()];
   QMessageBox::information(this,"", matakuliah);
   QString tb1 = ui->lineEdit_tb1->text();
   QString tb2 = ui->lineEdit_tb2->text();
   QString l1 = ui->lineEdit_l1->text();
   QString l2 = ui->lineEdit_l2->text();
   QString q1 = ui->lineEdit_q1->text();
   QString q2 = ui->lineEdit_q2->text();
   QString uts = ui->lineEdit_uts->text();
   QString uas = ui->lineEdit_uas->text();

   QSqlQuery* q = new QSqlQuery();
   q->prepare("insert into nilai (nim_mahasiswa, mk_id, tb1, tb2, l1, l2, q1, q2, uts, uas) values ('"+mahasiswa+"','"+matakuliah+"','"+tb1+"','"+tb2+"','"+l1+"','"+l2+"','"+q1+"','"+q2+"','"+uts+"','"+uas+"')");
   if(q->exec()){
       QMessageBox::information(this, "Berhasil", "Berhasil diatmbahkan!");
       refresh();
       return;
   }
   QMessageBox::warning(this, "Gagal", q->lastError().text());

}

void Nilai::on_pushButton_Update_clicked()
{
    QString mahasiswa = nim[ui->comboBox->currentIndex()];
    QString matakuliah = kod[ui->comboBox_2->currentIndex()];
    QMessageBox::information(this,"", matakuliah);
    QString tb1 = ui->lineEdit_tb1->text();
    QString tb2 = ui->lineEdit_tb2->text();
    QString l1 = ui->lineEdit_l1->text();
    QString l2 = ui->lineEdit_l2->text();
    QString q1 = ui->lineEdit_q1->text();
    QString q2 = ui->lineEdit_q2->text();
    QString uts = ui->lineEdit_uts->text();
    QString uas = ui->lineEdit_uas->text();

    QSqlQuery* q = new QSqlQuery();
    q->prepare("update nilai set  tb1 = '"+tb1+"', tb2 ='"+tb2+"',l1 ='"+l1+"', l2 ='"+l2+"', q1 ='"+q1+"',q2='"+q2+"',uts = '"+uts+"', uas = '"+uas+"' where nim_mahasiswa ='"+mahasiswa+"' and mk_id='"+matakuliah+"'");
    if(q->exec()){
        QMessageBox::information(this, "Berhasil", "Berhasil diupdate!");
        refresh();
        return;
    }
    QMessageBox::warning(this, "Gagal", q->lastError().text());
}

void Nilai::on_pushButton_Delete_clicked()
{
    QString mahasiswa = nim[ui->comboBox->currentIndex()];
    QString matakuliah = kod[ui->comboBox_2->currentIndex()];

    QSqlQuery* q = new QSqlQuery();
    q->prepare("delete from nilai where nim_mahasiswa ='"+mahasiswa+"' and mk_id='"+matakuliah+"'");
    if(q->exec()){
        QMessageBox::information(this, "Berhasil", "Berhasil didelete!");
        refresh();
        return;
    }
    QMessageBox::warning(this, "Gagal", q->lastError().text());
}

void Nilai::on_pushButton_Clear_clicked()
{
    refresh();
}

void Nilai::refresh(){
    ui->lineEdit_l1->clear();
    ui->lineEdit_l2->clear();
    ui->lineEdit_q1->clear();
    ui->lineEdit_q2->clear();
    ui->lineEdit_tb1->clear();
    ui->lineEdit_tb2->clear();
    ui->lineEdit_uas->clear();
    ui->lineEdit_uts->clear();
    ui->comboBox->clear();
    ui->comboBox_2->clear();

    QSqlQuery* q = new QSqlQuery();
    q->prepare("select matakuliah.nama_mk as Matakuliah, dosen.nama_dosen as Dosen, mahasiswa.nim as NIM, mahasiswa.nama as Mahasiswa, tb1 as TB1, tb2 as TB2, l1 as L1, l2 as L2, q1 as Q1, q2 as Q2, uts as UTS, uas as UAS from nilai join matakuliah on (nilai.mk_id = matakuliah.kode) join mahasiswa on (nilai.nim_mahasiswa=mahasiswa.nim) join dosen on (matakuliah.nip_dosen = dosen.nip)");
    q->exec();

    QSqlQueryModel* m = new QSqlQueryModel();
    m->setQuery(*q);
    ui->tableView->setModel(m);
    q->clear();
    q->prepare("select * from mahasiswa");
    q->exec();
    mh = 1;
    ui->comboBox->addItem("-Pilih Mahasiswa-");
    while(q->next()){
        QString k = q->value("nama").toString();
        QString l = q->value("nim").toString();
        ui->comboBox->addItem(k);
        mah[mh] = k;
        nim[mh] = l;
        mh++;
    }

    q->clear();
    q->prepare("select * from matakuliah");
    q->exec();

    mt = 1;
    ui->comboBox_2->addItem("-Pilih Matakuliah-");
    while(q->next()){
        QString k = q->value("kode").toString();
        QString l = q->value("nama_mk").toString();
        ui->comboBox_2->addItem(l);
        kod[mt] = k;
        mat[mt] = l;
        mt++;
    }
    ui->comboBox->setCurrentIndex(0);
    ui->comboBox_2->setCurrentIndex(0);
    ui->pushButton_Delete->setVisible(false);
    ui->pushButton_Update->setVisible(false);
    ui->pushButton_Tambah->setVisible(true);
    ui->comboBox_2->setDisabled(false);
    ui->comboBox->setDisabled(false);
}

void Nilai::on_tableView_clicked(const QModelIndex &index)
{
    QString matakuliah, mahasiswa, tb1, tb2, l1, l2, q1, q2, uts, uas;
    matakuliah = ui->tableView->model()->index(index.row(), 0).data().toString();
    mahasiswa = ui->tableView->model()->index(index.row(), 2).data().toString();
    tb1 = ui->tableView->model()->index(index.row(), 4).data().toString();
    tb2 = ui->tableView->model()->index(index.row(), 5).data().toString();
    l1 = ui->tableView->model()->index(index.row(), 6).data().toString();
    l2 = ui->tableView->model()->index(index.row(), 7).data().toString();
    q1 = ui->tableView->model()->index(index.row(), 8).data().toString();
    q2 = ui->tableView->model()->index(index.row(), 9).data().toString();
    uts = ui->tableView->model()->index(index.row(), 10).data().toString();
    uas = ui->tableView->model()->index(index.row(), 11).data().toString();

    int selectMk = 0;
    for(int i=0; i<mt; i++){
        if(mat[i]==matakuliah){
            selectMk = i;
        }
    }

    ui->comboBox_2->setCurrentIndex(selectMk);
    int selectMh = 0;
    for(int i=0; i<mh; i++){
        if(nim[i]==mahasiswa){
            selectMh = i;
        }
    }
    ui->comboBox->setCurrentIndex(selectMh);

    ui->lineEdit_tb1->setText(tb1);
    ui->lineEdit_tb2->setText(tb2);
    ui->lineEdit_l1->setText(l1);
    ui->lineEdit_l2->setText(l2);
    ui->lineEdit_q1->setText(q1);
    ui->lineEdit_q2->setText(q2);
    ui->lineEdit_uts->setText(uts);
    ui->lineEdit_uas->setText(uas);
    ui->pushButton_Delete->setVisible(true);
    ui->pushButton_Update->setVisible(true);
    ui->pushButton_Tambah->setVisible(false);
    ui->comboBox->setDisabled(true);
    ui->comboBox_2->setDisabled(true);
}
