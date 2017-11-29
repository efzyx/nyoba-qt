#ifndef DATAMAHASISWA_H
#define DATAMAHASISWA_H

#include <QDialog>
#include<mysqlconnect.h>
#include<QSqlDatabase>
namespace Ui {
class DataMahasiswa;
}

class DataMahasiswa : public QDialog
{
    Q_OBJECT

public:
    explicit DataMahasiswa(QWidget *parent = 0);
    ~DataMahasiswa();
    QSqlDatabase db;
    QString file;

private slots:
    void on_pushButton_pilih_clicked();

    void on_pushButton_clicked();

    void on_pushButton_delete_clicked();

    void refresh();

    void on_tableView_clicked(const QModelIndex &index);

    void clear();

    void on_pushButton_update_clicked();

    void on_pushButton_Clear_clicked();

private:
    Ui::DataMahasiswa *ui;
};

#endif // DATAMAHASISWA_H
