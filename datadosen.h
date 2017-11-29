#ifndef DATADOSEN_H
#define DATADOSEN_H

#include <QDialog>
#include<mainwindow.h>
#include<mysqlconnect.h>
#include<QSqlDatabase>
#include<QSqlQueryModel>

namespace Ui {
class DataDosen;
}

class DataDosen : public QDialog
{
    Q_OBJECT

public:
    explicit DataDosen(QWidget *parent = 0);
    ~DataDosen();
    QSqlDatabase db;
    QSqlQueryModel* m;
    MainWindow mw;


private slots:
    void on_pushButton_Close_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_tambah_clicked();

    void on_pushButton_delete_clicked();

    void refresh();

    void on_tableView_clicked(const QModelIndex &index);

    void clear();

    void on_pushButton_clear_clicked();

    void on_DataDosen_accepted();

private:
    Ui::DataDosen *ui;
    MainWindow* mainWindow;
};

#endif // DATADOSEN_H
