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
}

//функция добавления книги
void addBook(QSqlQuery &q, const QString &title, int year, const QVariant &authorId,
             const QVariant &genreId, int rating)
{
    q.addBindValue(title);
    q.addBindValue(year);
    q.addBindValue(authorId);
    q.addBindValue(genreId);
    q.addBindValue(rating);
    q.exec();
}

//функция добавления жанра
QVariant addGenre(QSqlQuery &q, const QString &name)
{
    q.addBindValue(name);
    q.exec();
    return q.lastInsertId();
}

//функция добавления автора
QVariant addAuthor(QSqlQuery &q, const QString &name, const QDate &birthdate)
{
    q.addBindValue(name);
    q.addBindValue(birthdate);
    q.exec();
    return q.lastInsertId();
}



//функция создания базы данных
void MainWindow::on_createButton_clicked()
{  
    //создаем базу данных
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("books.sqlite");

    //открываем базу данных
    if (!db.open())
        qDebug() << db.lastError();

    //получаем список таблиц
    QStringList tables = db.tables();

    //проверяем, есть ли таблицы в базе
    if (tables.contains("books"))
    {
        qDebug() << "В базу уже добавлены таблицы!";
        return;
    }

    //создаем запросы на создание таблиц
    QSqlQuery q;
    if (!q.exec(QLatin1String("create table books(id integer primary key, title varchar, author integer, genre integer, year integer, rating integer)")))
        qDebug() << q.lastError();
    if (!q.exec(QLatin1String("create table authors(id integer primary key, name varchar, birthdate date)")))
        qDebug() << q.lastError();
    if (!q.exec(QLatin1String("create table genres(id integer primary key, name varchar)")))
        qDebug() << q.lastError();

    //вставляем авторов в таблицу
    if (!q.prepare(QLatin1String("insert into authors(name, birthdate) values(?, ?)")))
           qDebug() << q.lastError();
     QVariant asimovId = addAuthor(q, QLatin1String("Isaac Asimov"), QDate(1920, 2, 1));
     QVariant greeneId = addAuthor(q, QLatin1String("Graham Greene"), QDate(1904, 10, 2));
     QVariant pratchettId = addAuthor(q, QLatin1String("Terry Pratchett"), QDate(1948, 4, 28));

     //вставляем жанры в таблицу
     if (!q.prepare(QLatin1String("insert into genres(name) values(?)")))
             qDebug() << q.lastError();
     QVariant sfiction = addGenre(q, QLatin1String("Science Fiction"));
     QVariant fiction = addGenre(q, QLatin1String("Fiction"));
     QVariant fantasy = addGenre(q, QLatin1String("Fantasy"));

     //вставляем книги в таблицу
     if (!q.prepare(QLatin1String("insert into books(title, year, author, genre, rating) values(?, ?, ?, ?, ?)")))
             qDebug() << q.lastError();
     addBook(q, QLatin1String("Foundation"), 1951, asimovId, sfiction, 3);
     addBook(q, QLatin1String("Foundation and Empire"), 1952, asimovId, sfiction, 4);
     addBook(q, QLatin1String("Second Foundation"), 1953, asimovId, sfiction, 3);
     addBook(q, QLatin1String("Foundation's Edge"), 1982, asimovId, sfiction, 3);
     addBook(q, QLatin1String("Foundation and Earth"), 1986, asimovId, sfiction, 4);
     addBook(q, QLatin1String("Prelude to Foundation"), 1988, asimovId, sfiction, 3);
     addBook(q, QLatin1String("Forward the Foundation"), 1993, asimovId, sfiction, 3);
     addBook(q, QLatin1String("The Power and the Glory"), 1940, greeneId, fiction, 4);
     addBook(q, QLatin1String("The Third Man"), 1950, greeneId, fiction, 5);
     addBook(q, QLatin1String("Our Man in Havana"), 1958, greeneId, fiction, 4);
     addBook(q, QLatin1String("Guards! Guards!"), 1989, pratchettId, fantasy, 3);
     addBook(q, QLatin1String("Night Watch"), 2002, pratchettId, fantasy, 3);
     addBook(q, QLatin1String("Going Postal"), 2004, pratchettId, fantasy, 3);
}

//представление view


void MainWindow::on_sqlView_activated(const QModelIndex &index)
{

}

//конвертация в csv-файл
void MainWindow::on_convertButton_clicked()
{
    //попробуем вычленить таблицы и заголовки
    QStringList tables = db.tables();

    for (auto it = tables.begin(); it != tables.end(); it++)
    {
        qDebug() << *it;
    }

    QSqlRecord fields = db.record(tables.at(2));

    qDebug() << fields.fieldName(0);
    qDebug() << fields.fieldName(1);
    qDebug() << fields.fieldName(2);
}
