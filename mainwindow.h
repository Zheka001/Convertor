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

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;    //база данных
    bool isDatabase = false;
};

//void addBook(QSqlQuery &q, const QString &title, int year, const QVariant &authorId,
             //const QVariant &genreId, int rating);

//QVariant addGenre(QSqlQuery &q, const QString &name);

//QVariant addAuthor(QSqlQuery &q, const QString &name, const QDate &birthdate);

QString specialProc(QString str);

#endif // MAINWINDOW_H
