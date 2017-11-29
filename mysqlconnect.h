#ifndef MYSQLCONNECT_H
#define MYSQLCONNECT_H

#include<QSqlDatabase>



class MysqlConnect
{
public:
    static QSqlDatabase getConnect(QString host, QString username, QString password, QString db){
        QSqlDatabase koneksi;
        koneksi = QSqlDatabase::addDatabase("QMYSQL");
        koneksi.setHostName(host);
        koneksi.setUserName(username);
        koneksi.setPassword(password);
        koneksi.setDatabaseName(db);
        return koneksi;
    }

    static void closeConnection(QSqlDatabase db){
        db.close();
        db.removeDatabase(db.connectionName());
    }
};

#endif // MYSQLCONNECT_H
