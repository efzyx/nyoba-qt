#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<mysqlconnect.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QSqlDatabase db;
    QString user;
    Ui::MainWindow *ui;
    int setData(const QString &labelText);
    int mk;
    QString kod[100], mat[100];


protected:
      void showEvent(QShowEvent *ev);

signals:
      void sigShowEvent();

private slots:
      void slotShowEvent();
      void on_actionDosen_triggered();
      void on_actionMahasiswa_triggered();
      void on_actionMatakuliah_triggered();
      void on_actionUser_triggered();
      void on_actionInput_triggered();
      void on_actionLaporan_triggered();
      void showNilai(QString s);
      void refreshMat();
//      void on_comboBox_currentIndexChanged(int index);
      void on_actionHalaman_Utama_triggered();
      void on_pushButtonRefresh_clicked();
      void on_pushButtonLoad_clicked();
      void on_pushButtonStatis_clicked();
      void load();
      void on_actionKeluar_triggered();
      void on_pushButtonPrint_clicked();
};

#endif // MAINWINDOW_H
