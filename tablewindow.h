#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QList>
#include <QAbstractTableModel>
#include <QSqlDatabase>

class TableWindow : public QAbstractTableModel
{
public:
    explicit TableWindow();

    void clear();
    void setData(QString str);
private:
    explicit TableWindow(QObject* parent = 0);

    int countRows;
    int countColumns;

    QList<QStringList> data;
};

#endif // TABLEWINDOW_H
