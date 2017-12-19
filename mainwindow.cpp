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
    QString table = ui->tableBox->currentText();

    //создаем csv файл с выбранной таблицей
    QFile fileCsv(table + ".csv");
    fileCsv.open(QIODevice::ReadWrite);
    QTextStream csv(&fileCsv);

    //запишем поля заголовков в файл
    QSqlRecord fields = db.record(table);

    QStringList str;
    for (int i = 0; i < fields.count(); i++)
    {
        //! необходимо реализовать обработку специальных символов
        str << specialProc(fields.fieldName(i));
    }
    csv << str.join(";") << endl;

    //запишем строчки таблицы в файл
    QSqlQuery q;
    q.exec("SELECT * from " + table);

    //Обрабатываем каждую строку результата запроса
    while(q.next())
    {
        str.clear();
        for (int i = 0; i < fields.count(); i++)
        {
            str << specialProc(q.value(i).toString());
        }
        csv << str.join(';') << endl;
    }

    fileCsv.close();
}

//тренируемся запоминать данные
void MainWindow::on_actionOpenDb_triggered()
{
    isDatabase = true;

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open database"), "D:\QT_project\converter\Debug\DB", tr("Databases files (*.sqlite)"));
    QString name = fileName.mid(fileName.lastIndexOf("/") + 1);

    //qDebug() << name;

    //открытие базы данных
    db = QSqlDatabase::addDatabase("QSQLITE");
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
}


void MainWindow::on_showButton_clicked()
{


    QString table = ui->tableBox->currentText();

    if (isDatabase)
    {
        QSqlQuery q;
        q.exec("SELECT * FROM " + table);


        QSqlRecord fieldsRec = db.record(table);

        QStringList fieldsStr;
        for (int i = 0; i < fieldsRec.count(); i++)
        {

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
    }
    else
    {
        //случай открытия CSV файла
    }
}

void MainWindow::on_actionOpencsv_triggered()
{
    isDatabase = false;

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open database"), "D:\QT_project\converter\Debug\DB", tr("Databases files (*.csv)"));
    QString name = fileName.mid(fileName.lastIndexOf("/") + 1);

    QString catName = name.replace(".csv", "");

    ui->tableBox->addItem(catName);
}
