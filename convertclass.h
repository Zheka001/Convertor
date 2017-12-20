#ifndef CONVERTCLASS_H
#define CONVERTCLASS_H

#include "QtSql"
#include "QFileDialog"
#include <processing.h>
#include <QTranslator>
#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include "QFileDialog"
#include <QStandardItemModel>
#include <QTreeView>

class convertclass
{
public:
    convertclass();
    ~convertclass(){ delete []types; }
    void convertToSql(QString filename);
private:
    QSqlDatabase dbMy;
    QString *types;
    QString fname,ex_cr,ex_in;
    void determineSchema (QString str);
    void generateQuary (QString str);
    void determineType();
};

#endif // CONVERTCLASS_H
