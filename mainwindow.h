#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlQuery>

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

    void on_actionOpenDb_triggered();

//    void on_convertSqlButton_clicked();

    void on_showButton_clicked();

    void on_actionOpencsv_triggered();

    void on_convertSqlButton_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;    //база данных
    QString name;
    bool isDatabase = false;
};

QString specialProc(QString str);
QString whatType(QString str);
QStringList parseStr(QString str);
QString withoutQ(QString str);

#endif // MAINWINDOW_H
