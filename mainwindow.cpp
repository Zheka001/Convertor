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

    ui->showButton->hide();
    ui->tableBox->hide();
    ui->convertButton->hide();
    ui->convertSqlButton->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}


//конвертация в csv-файл
void MainWindow::on_convertButton_clicked()
{
    QString table = ui->tableBox->currentText();

    converter.convertToCSV(table);
    /*
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
        str << processingForCsvStr(fields.fieldName(i));
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
            str << processingForCsvStr(q.value(i).toString());
        }
        csv << str.join(';') << endl;
    }

    fileCsv.close();
    */

    ui->statusBar->showMessage("Файл конвертирован", 5000);
}

void MainWindow::on_convertSqlButton_clicked()
{

    converter.convertToSQL();
    /*
    QFile file(name);
    if ( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File not exists";
    }
    else
    {
        // Создаём поток для извлечения данных из файла
        QTextStream in(&file);
        QString line = in.readLine();
        QString line2 = in.readLine();
        QStringList parse = parseStr(line);
        QStringList parse2 = parseStr(line2);

        QString *types = new QString[parse.size()];

        int i = 0;
        for (QString item : parse2)
        {
            types[i++] = whatType(item);
        }

        i = 0;

        QString ex_cr("create table T1(");
        QString ex_in("insert into T1(");
        QString ex_v(") values(");
        QString *title = new QString[parse.size()] ;

        for (QString item : parse)
        {
            title[i] = item;
            ex_cr += item+" ";
            ex_in += item+", ";
            ex_v += "?, ";
            ex_cr += types[i++]+", ";
        }
        ex_in.remove(ex_in.size()-2,2);
        ex_cr.remove(ex_cr.size()-2,2);
        ex_v.remove(ex_v.size()-2,2);
        ex_in += ex_v + ")";
        ex_cr+=")";

        QString table = ui->tableBox->currentText();
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(table + ".sqlite");

        //открываем базу данных
        if (!db.open())
            qDebug() << db.lastError();

        //получаем список таблиц
        QStringList tables = db.tables();

        QSqlQuery q;
        //проверяем, есть ли таблицы в базе
        if (!tables.empty())
        {
            qDebug() << "Already have tables!";
            QSqlRecord schema = db.record(tables.at(0));
            if (schema.count() == parseStr(line).count() )
            {
                int j = 0;
                for (QString item : parseStr(line))
                {
                    if (item == schema.fieldName(j))
                        j++;
                    else
                        break;
                }

                if ( j == schema.count() )
                {
                    q.exec(QString("DELETE FROM "+tables.at(0)+";"));
                    qDebug() << "Truncate!";
                }
                else
                {
                    q.exec(QString("DROP TABLE "+tables.at(0)+";"));
                    if (!q.exec(ex_cr))
                        qDebug() << q.lastError();
                }
            }
            else
            {
                q.exec(QString("DROP TABLE "+tables.at(0)+";"));
                if (!q.exec(ex_cr))
                    qDebug() << q.lastError();
            }
        }
        else
            if (!q.exec(ex_cr))
                qDebug() << q.lastError();



        if (!q.prepare(ex_in))
               qDebug() << q.lastError();
        for (QString item : parse2)
        {
            q.addBindValue(item);
        }
        q.exec();

        // Считываем данные до конца файла
        while (!in.atEnd())
        {
            line = in.readLine();

            for (QString item : parseStr(line))
            {
                q.addBindValue(item);
            }
            q.exec();
        }
        file.close();
        db.close();
    }
    qDebug() << "Done";

    */
    ui->statusBar->showMessage("Файл конвертирован", 5000);
}

//тренируемся запоминать данные
void MainWindow::on_actionOpenDb_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,"Open File", "", tr("Databases files (*.sqlite)"), Q_NULLPTR, QFileDialog::DontConfirmOverwrite);
    QString name = fileName.mid(fileName.lastIndexOf("/") + 1);

    //qDebug() << name;

    converter.setDatabase(db, ui->tableBox->currentText());

    //открытие базы данных
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(name);
    db.open();

    //список таблиц
    QStringList tables = db.tables();

    //заполнение комбобокса
    if (!tables.empty())
    {
        ui->tableBox->clear();
        ui->tableBox->addItems(tables);
    }

    db.close();


    ui->showButton->show();
    ui->tableBox->show();
    ui->convertButton->show();
    ui->convertSqlButton->hide();
}


void MainWindow::on_showButton_clicked()
{
    QString table = ui->tableBox->currentText();
    TableViewer tv;

    //операция с базой данных
    if (converter.getCondition())
    {
        tv.setData(converter.getDatabase(), table);
        ui->sqlView->setModel(tv.returnModel());
    }
    //если работаем с файлом
    else
    {
        tv.setData(converter.getFile());
        ui->sqlView->setModel(tv.returnModel());
    }
}

void MainWindow::on_actionOpencsv_triggered()
{
    //isDatabase = false;

    QString fileName = QFileDialog::getOpenFileName(this,tr("Open database"), "E:\qt_projects\convert", tr("Databases files (*.csv)"));

    // не факт, что нужно обрезать
    QString name = fileName.mid(fileName.lastIndexOf("/") + 1);
    converter.setFile(name);

    QString catName = name;
    catName.replace(".csv", "");

    ui->tableBox->clear();
    ui->tableBox->addItem(catName);

    ui->showButton->show();
    ui->tableBox->show();
    ui->convertButton->hide();
    ui->convertSqlButton->show();
}

