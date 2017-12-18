#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include <QStandardItemModel>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QRegExp>
#include "QFileDialog"
#include <QItemSelectionModel>
#include <QTreeView>
#include <QSqlRecord>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *csvModel;
};

QString whatType(QString str);
QStringList parseStr(QString str);
QString withoutQ(QString str);

#endif // MAINWINDOW_H
