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
    //int rowCount(const int )
private:
    //explicit TableWindow(QObject* parent = 0);

    int rowCount = 0;
    int ColumnCount= 0;

    QList<QStringList> data;
};

#endif // TABLEWINDOW_H
