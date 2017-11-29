#ifndef NILAI_H
#define NILAI_H

#include <QDialog>
#include<QSqlDatabase>

namespace Ui {
class Nilai;
}

class Nilai : public QDialog
{
    Q_OBJECT

public:
    explicit Nilai(QWidget *parent = 0);
    ~Nilai();
    QSqlDatabase db;
    QString mah[100], nim[100], mat[100], kod[100];
    int mh, mt;

private slots:
    void on_pushButton_Tambah_clicked();

    void on_pushButton_Update_clicked();

    void on_pushButton_Delete_clicked();

    void on_pushButton_Clear_clicked();

    void refresh();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Nilai *ui;
};

#endif // NILAI_H
