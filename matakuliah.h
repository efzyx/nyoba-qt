#ifndef MATAKULIAH_H
#define MATAKULIAH_H

#include <QDialog>
#include<QSqlDatabase>

namespace Ui {
class Matakuliah;
}

class Matakuliah : public QDialog
{
    Q_OBJECT

public:
    explicit Matakuliah(QWidget *parent = 0);
    ~Matakuliah();
    QSqlDatabase db;
    int in;
    QString dos[100];
    QString nip[100];

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void refresh();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Matakuliah *ui;
};

#endif // MATAKULIAH_H
