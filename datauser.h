#ifndef DATAUSER_H
#define DATAUSER_H

#include <QDialog>
#include<QSqlDatabase>

namespace Ui {
class DataUser;
}

class DataUser : public QDialog
{
    Q_OBJECT

public:
    explicit DataUser(QWidget *parent = 0);
    ~DataUser();
    QSqlDatabase db;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void refresh();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::DataUser *ui;
};

#endif // DATAUSER_H
