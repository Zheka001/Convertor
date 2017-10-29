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
    void on_createButton_clicked();

    void on_sqlView_activated(const QModelIndex &index);

    void on_convertButton_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase db;
};

void addBook(QSqlQuery &q, const QString &title, int year, const QVariant &authorId,
             const QVariant &genreId, int rating);

QVariant addGenre(QSqlQuery &q, const QString &name);

QVariant addAuthor(QSqlQuery &q, const QString &name, const QDate &birthdate);

#endif // MAINWINDOW_H
