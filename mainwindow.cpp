#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QSqlQuery>
#include<QSqlError>
#include<QSqlDatabase>
#include<QMessageBox>
#include<login.h>
#include<datadosen.h>
#include<datamahasiswa.h>
#include<matakuliah.h>
#include<datauser.h>
#include<nilai.h>
#include<QSqlQueryModel>
#include<QtPrintSupport/QPrinter>
#include<QPainter>
#include<QPdfWriter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(this, SIGNAL(sigShowEvent()), this, SLOT(slotShowEvent()));
    setWindowModality(Qt::ApplicationModal);
    ui->lineEditUser->hide();
    db = QSqlDatabase::database();
    ui->groupBox->close();



}

MainWindow::~MainWindow()
{
    delete ui;
}

MainWindow::setData(const QString &labelText){
    ui->lineEditUser->setText(labelText);
    return true;
}

void MainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    emit sigShowEvent();
}

void MainWindow::slotShowEvent()
{
    user = ui->lineEditUser->text();
    ui->labelWelcome->setText("Selamat Datang "+user);
    db = QSqlDatabase::database();
    if(!db.open()){
        QMessageBox::warning(this, "Login", db.lastError().text());
        return;
    }

    QSqlQuery* q = new QSqlQuery();
    q->prepare("select * from user where username='"+user+"'");
    q->exec();
    q->first();
    QString role = q->value("group_akses").toString();
    if(role == "1"){
        ui->menuInput_Data->menuAction()->setVisible(true);
        ui->actionInput->setVisible(true);
        ui->actionLaporan->setVisible(true);
    }else if(role == "2"){
        ui->menuInput_Data->menuAction()->setVisible(false);
    }else if(role == "3"){
        ui->menuInput_Data->menuAction()->setVisible(false);
        ui->actionInput->setVisible(false);
    }else{
        ui->menuInput_Data->menuAction()->setVisible(false);
        ui->actionInput->setVisible(false);
        ui->actionLaporan->setVisible(false);
    }
    q->clear();


}

void MainWindow::on_actionDosen_triggered()
{
    DataDosen *dd = new DataDosen();
//    hide();
    dd->setModal(true);
    dd->exec();
}

void MainWindow::on_actionMahasiswa_triggered()
{
    DataMahasiswa* dm = new DataMahasiswa(0);
    dm->setModal(true);
    dm->show();
}

void MainWindow::on_actionMatakuliah_triggered()
{
    Matakuliah *mk = new Matakuliah();
    mk->setModal(true);
    mk->show();
}

void MainWindow::on_actionUser_triggered()
{
    DataUser* du = new DataUser();
    du->setModal(true);
    du->show();
}

void MainWindow::on_actionInput_triggered()
{
    Nilai* n = new Nilai();
    n->setModal(true);
    n->show();
}

void MainWindow::on_actionLaporan_triggered()
{

    ui->groupBox->show();
    ui->calendarWidget->close();
    ui->labelWelcome->close();
    refreshMat();
}

void MainWindow::showNilai(QString s){
    QSqlQuery* q = new QSqlQuery();
    q->prepare(
                "SELECT `matakuliah`.`kode` as Kode, `matakuliah`.`nama_mk` as Matakuliah, `dosen`.`nama_dosen` as Dosen, (tb1+tb2+l1+l2+q1+q2)/6 as Tugas, uts as UTS, uas as UAS,((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas)as Angka , case ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 85 WHEN 1 then 'A' ELSE case ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 80 WHEN 1 then 'A-' else CASE ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 75 WHEN 1 THEN 'B+' else case ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 70 when 1 THEN 'B' ELSE case ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 65 when 1 THEN 'C+' else case ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 55 when 1 then 'C' else case ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas)>=40 when 1 then 'D' ELSE 'E' end end end end end end end as Huruf FROM `nilai` join `matakuliah` on (`matakuliah`.`kode` = `nilai`.`mk_id`) join `dosen` on(`dosen`.`nip` = `matakuliah`.`nip_dosen`) where kode='"+s+"'"
                );
    q->exec();
    QSqlQueryModel* m = new QSqlQueryModel();
    m->setQuery(*q);
    ui->tableView->setModel(m);
    q->clear();
}

void MainWindow::refreshMat(){

    QSqlQuery* q = new QSqlQuery();
    q->prepare("select * from matakuliah");
    q->exec();
    mk = 1;
    ui->comboBox->clear();
    ui->comboBox->addItem("-Pilih Matakuliah-");
    while (q->next()) {
        kod[mk]=q->value("kode").toString();
        mat[mk]=q->value("nama_mk").toString();
        ui->comboBox->addItem(q->value("nama_mk").toString());
        mk++;
    }
    q->clear();
    q->prepare("select * from mahasiswa");
    q->exec();
    ui->comboBox_3->clear();
    ui->comboBox_3->addItem("-Pilih SMU-");
    while (q->next()) {
        ui->comboBox_3->addItem(q->value("smu").toString());
    }

    ui->comboBox_2->clear();
    ui->comboBox_2->addItem("-Pilih Jenis Kelamin-");
    ui->comboBox_2->addItem("Laki-laki");
    ui->comboBox_2->addItem("Perempuan");
}

//void MainWindow::on_comboBox_currentIndexChanged(int index)
//{
//    QString s = kod[index];
//    showNilai(s);
//    ui->comboBox->currentData().clear();
//}

void MainWindow::on_actionHalaman_Utama_triggered()
{
    ui->calendarWidget->show();
    ui->labelWelcome->show();
    ui->groupBox->close();

}

void MainWindow::on_pushButtonRefresh_clicked()
{
    refreshMat();
}

void MainWindow::on_pushButtonLoad_clicked()
{
    load();
}

void MainWindow::load(){
    QString s = kod[ui->comboBox->currentIndex()];
    showNilai(s);
//    QTableWidgetItem *it = new QTableWidgetItem(tr("Hahaga").arg(
//              (1)*(1)));
//    ui->tableWidget->setItem(0,0,it);
}



void MainWindow::on_pushButtonStatis_clicked()
{
    QString smu, jk;
    QSqlQuery* q = new QSqlQuery();
    smu = ui->comboBox_3->currentText();
    jk = ui->comboBox_2->currentText();
    if(ui->comboBox_2->currentIndex() != 0 && ui->comboBox_3->currentIndex() != 0){
        q->exec("select count(*) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 85 and jk='"+jk+"' and smu='"+smu+"'");
//        QMessageBox::information(0, "", q->lastError().text());
        q->first();
        int a = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 80 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 85 and jk='"+jk+"' and smu='"+smu+"'");
        q->first();
        int amin = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 75 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 80 and jk='"+jk+"' and smu='"+smu+"'");
        q->first();
        int bplus = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 70 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 75 and jk='"+jk+"' and smu='"+smu+"'");
        q->first();
        int b = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 65 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 70 and jk='"+jk+"' and smu='"+smu+"'");
        q->first();
        int bmin = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 60 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 65 and jk='"+jk+"' and smu='"+smu+"'");
        q->first();
        int cplus = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 55 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 60 and jk='"+jk+"' and smu='"+smu+"'");
        q->first();
        int c = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 40 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 55 and jk='"+jk+"' and smu='"+smu+"'");
        q->first();
        int d = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 40 and jk='"+jk+"' and smu='"+smu+"'");
        q->first();
        int e = q->value("c").toInt();


        ui->tableWidget->setRowCount(9);
        ui->tableWidget->setColumnCount(2);
        QStringList sl;
        sl.append("Nilai A");
        sl.append("Nilai A-");
        sl.append("Nilai B+");
        sl.append("Nilai B");
        sl.append("Nilai B-");
        sl.append("Nilai C+");
        sl.append("Nilai C");
        sl.append("Nilai D");
        sl.append("Nilai E");
        ui->tableWidget->setVerticalHeaderLabels(sl);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Persentase" << "Jumlah");
        int nil[9] = {a, amin, bplus, b, bmin, cplus, c, d, e};
        int jumlah =0;
        for(int u = 0; u < 9; u++){
            jumlah += nil[u];
            QString qs = QString::number(nil[u]);
            qs.constData();
            QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
            ui->tableWidget->setItem(u,1,it);
        }
        if(jumlah!=0){
            int nilp[9] = {a/jumlah*100, amin/jumlah*100, bplus/jumlah*100, b/jumlah*100, bmin/jumlah*100, cplus/jumlah*100, c/jumlah*100, d/jumlah*100, e/jumlah*100};
            for(int u = 0; u < 9; u++){
                QString qs = QString::number(nilp[u])+" %";
                qs.constData();
                QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
                ui->tableWidget->setItem(u,0,it);
            }
        }else{
            int nilp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for(int u = 0; u < 9; u++){
                QString qs = QString::number(nilp[u])+" %";
                qs.constData();
                QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
                ui->tableWidget->setItem(u,0,it);
            }
        }


    }else if(ui->comboBox_2->currentIndex() == 0 && ui->comboBox_3->currentIndex() != 0){
        q->exec("select count(*) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 85 and smu='"+smu+"'");
//        QMessageBox::information(0, "", q->lastError().text());
        q->first();
        int a = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 80 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 85 and smu='"+smu+"'");
        q->first();
        int amin = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 75 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 80 and smu='"+smu+"'");
        q->first();
        int bplus = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 70 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 75 and smu='"+smu+"'");
        q->first();
        int b = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 65 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 70 and smu='"+smu+"'");
        q->first();
        int bmin = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 60 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 65  and smu='"+smu+"'");
        q->first();
        int cplus = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 55 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 60  and smu='"+smu+"'");
        q->first();
        int c = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 40 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 55 and smu='"+smu+"'");
        q->first();
        int d = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 40 and smu='"+smu+"'");
        q->first();
        int e = q->value("c").toInt();


        ui->tableWidget->setRowCount(9);
        ui->tableWidget->setColumnCount(2);
        QStringList sl;
        sl.append("Nilai A");
        sl.append("Nilai A-");
        sl.append("Nilai B+");
        sl.append("Nilai B");
        sl.append("Nilai B-");
        sl.append("Nilai C+");
        sl.append("Nilai C");
        sl.append("Nilai D");
        sl.append("Nilai E");
        ui->tableWidget->setVerticalHeaderLabels(sl);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Persentase" << "Jumlah");
        int nil[9] = {a, amin, bplus, b, bmin, cplus, c, d, e};
        int jumlah =0;
        for(int u = 0; u < 9; u++){
            jumlah += nil[u];
            QString qs = QString::number(nil[u]);
            qs.constData();
            QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
            ui->tableWidget->setItem(u,1,it);
        }
        if(jumlah!=0){
            int nilp[9] = {a/jumlah*100, amin/jumlah*100, bplus/jumlah*100, b/jumlah*100, bmin/jumlah*100, cplus/jumlah*100, c/jumlah*100, d/jumlah*100, e/jumlah*100};
            for(int u = 0; u < 9; u++){
                QString qs = QString::number(nilp[u])+" %";
                qs.constData();
                QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
                ui->tableWidget->setItem(u,0,it);
            }
        }else{
            int nilp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for(int u = 0; u < 9; u++){
                QString qs = QString::number(nilp[u])+" %";
                qs.constData();
                QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
                ui->tableWidget->setItem(u,0,it);
            }
        }


    }else if(ui->comboBox_2->currentIndex() != 0 && ui->comboBox_3->currentIndex() == 0){
        q->exec("select count(*) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 85 and jk='"+jk+"'");
//        QMessageBox::information(0, "", q->lastError().text());
        q->first();
        int a = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 80 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 85 and jk='"+jk+"'");
        q->first();
        int amin = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 75 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 80 and jk='"+jk+"'");
        q->first();
        int bplus = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 70 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 75 and jk='"+jk+"'");
        q->first();
        int b = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 65 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 70 and jk='"+jk+"'");
        q->first();
        int bmin = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 60 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 65 and jk='"+jk+"'");
        q->first();
        int cplus = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 55 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 60 and jk='"+jk+"'");
        q->first();
        int c = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 40 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 55 and jk='"+jk+"'");
        q->first();
        int d = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 40 and jk='"+jk+"'");
        q->first();
        int e = q->value("c").toInt();


        ui->tableWidget->setRowCount(9);
        ui->tableWidget->setColumnCount(2);
        QStringList sl;
        sl.append("Nilai A");
        sl.append("Nilai A-");
        sl.append("Nilai B+");
        sl.append("Nilai B");
        sl.append("Nilai B-");
        sl.append("Nilai C+");
        sl.append("Nilai C");
        sl.append("Nilai D");
        sl.append("Nilai E");
        ui->tableWidget->setVerticalHeaderLabels(sl);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Persentase" << "Jumlah");
        int nil[9] = {a, amin, bplus, b, bmin, cplus, c, d, e};
        int jumlah =0;
        for(int u = 0; u < 9; u++){
            jumlah += nil[u];
            QString qs = QString::number(nil[u]);
            qs.constData();
            QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
            ui->tableWidget->setItem(u,1,it);
        }
        if(jumlah!=0){
            int nilp[9] = {a/jumlah*100, amin/jumlah*100, bplus/jumlah*100, b/jumlah*100, bmin/jumlah*100, cplus/jumlah*100, c/jumlah*100, d/jumlah*100, e/jumlah*100};
            for(int u = 0; u < 9; u++){
                QString qs = QString::number(nilp[u])+" %";
                qs.constData();
                QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
                ui->tableWidget->setItem(u,0,it);
            }
        }else{
            int nilp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for(int u = 0; u < 9; u++){
                QString qs = QString::number(nilp[u])+" %";
                qs.constData();
                QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
                ui->tableWidget->setItem(u,0,it);
            }
        }


    }else {
        q->exec("select count(*) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 85");
//        QMessageBox::information(0, "", q->lastError().text());
        q->first();
        int a = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 80 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 85");
        q->first();
        int amin = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 75 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 80");
        q->first();
        int bplus = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 70 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 75");
        q->first();
        int b = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 65 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 70");
        q->first();
        int bmin = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 60 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 65");
        q->first();
        int cplus = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 55 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 60");
        q->first();
        int c = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) >= 40 and ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 55");
        q->first();
        int d = q->value("c").toInt();
        q->clear();
        q->exec("select count(nama) as c from mahasiswa join nilai on (nilai.nim_mahasiswa = mahasiswa.nim) where ((tb1+tb2+l1+l2+q1+q2)/6*0.4 + 0.25*uts + 0.35*uas) < 40");
        q->first();
        int e = q->value("c").toInt();


        ui->tableWidget->setRowCount(9);
        ui->tableWidget->setColumnCount(2);
        QStringList sl;
        sl.append("Nilai A");
        sl.append("Nilai A-");
        sl.append("Nilai B+");
        sl.append("Nilai B");
        sl.append("Nilai B-");
        sl.append("Nilai C+");
        sl.append("Nilai C");
        sl.append("Nilai D");
        sl.append("Nilai E");
        ui->tableWidget->setVerticalHeaderLabels(sl);
        ui->tableWidget->setHorizontalHeaderLabels(QStringList()<<"Persentase" << "Jumlah");
        int nil[9] = {a, amin, bplus, b, bmin, cplus, c, d, e};
        int jumlah =0;
        for(int u = 0; u < 9; u++){
            jumlah += nil[u];
            QString qs = QString::number(nil[u]);
            qs.constData();
            QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
            ui->tableWidget->setItem(u,1,it);
        }
        if(jumlah!=0){
            int nilp[9] = {a/jumlah*100, amin/jumlah*100, bplus/jumlah*100, b/jumlah*100, bmin/jumlah*100, cplus/jumlah*100, c/jumlah*100, d/jumlah*100, e/jumlah*100};
            for(int u = 0; u < 9; u++){
                QString qs = QString::number(nilp[u])+" %";
                qs.constData();
                QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
                ui->tableWidget->setItem(u,0,it);
            }
        }else{
            int nilp[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
            for(int u = 0; u < 9; u++){
                QString qs = QString::number(nilp[u])+" %";
                qs.constData();
                QTableWidgetItem *it = new QTableWidgetItem(qs.arg((1)*(1)));
                ui->tableWidget->setItem(u,0,it);
            }
        }


    }
}


void MainWindow::on_actionKeluar_triggered()
{
    Login* l = new Login();
    this->close();
    l->show();
}


void MainWindow::on_pushButtonPrint_clicked()
{
    QPdfWriter pdf("report.pdf");
    QPainter painter(&pdf);
    painter.setPen(Qt::black);
    painter.drawText(100, 0, "Test");
    painter.end();
    QMessageBox::information(this, "Report", "Report telah disimpan dalam bentuk PDF!");
}
