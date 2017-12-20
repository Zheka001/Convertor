#ifndef TABLEVIEWER_H
#define TABLEVIEWER_H

#include "QStandardItemModel"
#include "QSqlDatabase"
#include "QFile"
#include <QtSql>
#include <QSqlQuery>
#include "processing.h"

class TableViewer
{
public:
    TableViewer();

    void setData(QSqlDatabase& db, QString table);

    void setData(QString name);

    QStandardItemModel* returnModel();

private:
    QStandardItemModel *model;
};

#endif // TABLEVIEWER_H
