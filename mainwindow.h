#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <processing.h>
#include <convertclass.h>
#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>
#include "QFileDialog"
#include <QStandardItemModel>
#include <QTreeView>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_convertButton_clicked();
    void on_convertSqlButton_clicked();

    void on_actionOpenDb_triggered();
    void on_actionOpencsv_triggered();

    void on_showButton_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;    //база данных
    QString name;
    bool isDatabase = false;
};



#endif // MAINWINDOW_H
