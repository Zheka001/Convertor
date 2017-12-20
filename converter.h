#ifndef CONVERTER_H
#define CONVERTER_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include "tableviewer.h"

class Converter
{
public:
    Converter();

    ~Converter();

    void setDatabase(QSqlDatabase database, QString tableName);

    void setFile(QString fileName);

    QSqlDatabase getDatabase();

    QString getTable();

    QString getFile();

    bool getCondition();

    void convertToCSV(QString& tableName);

    void convertToSQL();

private:
    QSqlDatabase db;    //база данных
    QString name;
    QString table;
    bool isDatabase = false;
};

#endif // CONVERTER_H
