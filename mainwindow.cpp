#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QFileDialog"
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QTreeView>


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

//функция обработки строк
QString specialProc(QString str)
{
    if (str.contains(";") || str.contains(",") || str.contains("\"") || str.contains("\n"))
        return "\"" + str + "\"";

    if (str.contains("\""))
    {
        str.replace("\"","\"\"");
    }
    return str;
}


//конвертация в csv-файл
void MainWindow::on_convertButton_clicked()
{
    /*
    //создаем список таблиц базы данных
    QStringList tables = db.tables();


    //! необходимо выводить на экран все таблицы и выбирать среди них
    QString myTable = tables.at(0);

    //создаем csv файл с выбранной таблицей
    QFile fileCsv(myTable + ".csv");
    fileCsv.open(QIODevice::ReadWrite);
    QTextStream csv(&fileCsv);

    //запишем поля заголовков в файл
    QSqlRecord fields = db.record(myTable);

    QStringList str;
    for (int i = 0; i < fields.count(); i++)
    {
        //! необходимо реализовать обработку специальных символов
        str << specialProc(fields.fieldName(i));
    }
    csv << str.join(";") << endl;

    //запишем строчки таблицы в файл
    QSqlQuery q;
    q.exec("SELECT * from " + myTable);

    qDebug() << "Проверка на SELECT: " << q.isSelect();
    qDebug() << "Количество строк в таблице: " << q.size();
    qDebug() << "Текст в таблице: " << q.lastQuery();

    //Обрабатываем каждую строку результата запроса
    while(q.next())
    {
        str.clear();
        for (int i = 0; i < fields.count(); i++)
        {
            //! необходимо поставить обработку специальных символов
            str << specialProc(q.value(i).toString());
        }
        csv << str.join(';') << endl;
    }

    qDebug() << "Проверка кавычек: \" \"\" ";

    //qDebug() << str.join(";");

    fileCsv.close();
    */
}

//тренируемся запоминать данные
void MainWindow::on_actionOpenDb_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Open database"),"D:\QT_project\converter\Debug\DB", tr("Databases files (*.sqlite)"));
    QString name = fileName.mid(fileName.lastIndexOf("/") + 1);

    //qDebug() << name;

    //открытие базы данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    db.open();

    //список таблиц
    QStringList tables = db.tables();

    //заполнение комбобокса
    if (!tables.empty())
    {
        ui->tableBox->addItems(tables);
    }

    //скопировали из нижней, т.к. нет базы
    QString table = ui->tableBox->currentText();

    QSqlQuery q;
    q.exec("SELECT * FROM " + table);


    QSqlRecord fieldsRec = db.record(table);

    QStringList fieldsStr;
    for (int i = 0; i < fieldsRec.count(); i++)
    {
        //! необходимо реализовать обработку специальных символов
        fieldsStr << specialProc(fieldsRec.fieldName(i));
    }


    QStandardItemModel* model = new QStandardItemModel(this);
    model->setColumnCount(fieldsRec.count());
    model->setHorizontalHeaderLabels(fieldsStr);

    ui->sqlView->setModel(model);

    //Обрабатываем каждую строку результата запроса
    while(q.next())
    {
       QStringList rowList;
       QList<QStandardItem*> qStandItemList;
       for (int i = 0; i < fieldsRec.count(); i++)
       {
           qStandItemList.append(new QStandardItem(q.value(i).toString()));
       }
       model->insertRow(model->rowCount(),qStandItemList);
    }

    db.close();
}


void MainWindow::on_showButton_clicked()
{
   /*String table = ui->tableBox->currentText();

   QSqlQuery q;
   q.exec("SELECT * FROM " + table);

   QStandardItemModel model;

   QSqlRecord fields =
   int number  = 0;
   //Обрабатываем каждую строку результата запроса
   while(q.next())
   {
      QStringList rowList;
      for (int i = 0; i < fields.count(); i++)
      {
          rowList <<
      }
       model.insertRow(number, );
   }

*/
}
