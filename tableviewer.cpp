#include "tableviewer.h"

TableViewer::TableViewer()
{

}

void TableViewer::setData(QSqlDatabase &db, QString table)
{
    QSqlQuery q;
    q.exec("SELECT * FROM " + table);

    QSqlRecord fieldsRec = db.record(table);

    QStringList fieldsStr;
    for (int i = 0; i < fieldsRec.count(); i++)
    {
        fieldsStr << fieldsRec.fieldName(i);
    }

    model = new QStandardItemModel();
    model->setColumnCount(fieldsRec.count());
    model->setHorizontalHeaderLabels(fieldsStr);

    //MainWindow::ui->sqlView->setModel(model);

    //Обрабатываем каждую строку результата запроса
    while(q.next())
    {
        QList<QStandardItem*> qStandItemList;
        for (int i = 0; i < fieldsRec.count(); i++)
        {
            qStandItemList.append(new QStandardItem(q.value(i).toString()));
        }
        model->insertRow(model->rowCount(),qStandItemList);
    }
}

void TableViewer::setData(QString name)
{
    //случай открытия CSV файла
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

        QStringList title = parseStr(line);
        model = new QStandardItemModel();
        model->setColumnCount(title.count());
        model->setHorizontalHeaderLabels(title);

        //ui->sqlView->setModel(model);

        while (!in.atEnd())
        {
            line = in.readLine();
            QList<QStandardItem*> qStandItemList;

            for (QString item : parseStr(line))
            {
                qStandItemList.append(new QStandardItem(item));
            }
             model->insertRow(model->rowCount(),qStandItemList);
        }
        file.close();
    }
}

QStandardItemModel* TableViewer::returnModel()
{
    return model;
}


