#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dBase;
}

//функция создания базы данных
void MainWindow::on_createButton_clicked()
{  
    //создаем базу данных
    dBase = QSqlDatabase::addDatabase("QSQLITE");
    dBase.setDatabaseName("first_dbase.sqlite");

    //пытаемся ее открыть
    if (!dBase.open())
    {
        qDebug() << "Не удается открыть базу данных!";
        qDebug() << dBase.lastError().text();
        return;
    }

    //создаем запрос
    QSqlQuery queryT(dBase);
    QString str_create = "CREATE TABLE my_table ("
                  "number integer PRIMARY KEY NOT NULL, "
                  "address VARCHAR(255), "
                  "age integer"
                  ");";

    //исполняем запрос
    bool b = queryT.exec(str_create);
    if (!b)
    {
        qDebug() << "Не удается создать таблицу!";
        qDebug() << dBase.lastError().text();
    }

    //создаем запрос на вставку строки в таблицу
    QString str_insert = "INSERT INTO my_table(number, address, age) "
            "VALUES (%1, '%2', %3);";

    //вставляем недостающие элементы
    QString str_cr = str_insert.arg("1")
            .arg("Nauchnaya")
            .arg("12");

    //вставляем строку в таблице
    b = queryT.exec(str_cr);
    if (!b)
    {
        qDebug() << "Не удается вставить строку!";
        qDebug() << dBase.lastError().text();
    }

    //queryT.exec("SELECT * FROM my_table");

    //while (queryT.next())
    //{
    //    qDebug() << queryT.value("1").toInt();
    //}

    //dBase.close();
}

//представление view


void MainWindow::on_sqlView_activated(const QModelIndex &index)
{

}

//конвертация в csv-файл
void MainWindow::on_convertButton_clicked()
{
    //! будет выбрана откуда-то


    //пытаемся ее открыть
    //! допустим, что открыта
//    if (!dBase.open())
//    {
//        qDebug() << "Не удается открыть базу данных!";
//        qDebug() << dBase.lastError().text();
//        return;
//    }


    QSqlTableModel model;

    model.setTable("my_table");
    model.select();
    model.setEditStrategy(QSqlTableModel::OnFieldChange);

    ui->sqlView->setModel(&model);
    ui->sqlView->show();
}
