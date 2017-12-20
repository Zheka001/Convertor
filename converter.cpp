#include "converter.h"

Converter::Converter()
{

}

Converter::~Converter()
{

}

void Converter::setDatabase(QSqlDatabase database, QString tableName)
{
    db = database;
    table = tableName;
    isDatabase = true;
}

void Converter::setFile(QString fileName)
{
    name = fileName;
    isDatabase = false;
}

QSqlDatabase Converter::getDatabase()
{
    return db;
}

QString Converter::getTable()
{
   return table;
}

QString Converter::getFile()
{
    return name;
}

bool Converter::getCondition()
{
    return isDatabase;
}


void Converter::convertToCSV(QString& tableName)
{
    table = tableName;

    //создаем csv файл с выбранной таблицей
    QFile fileCsv(table + ".csv");
    fileCsv.open(QIODevice::ReadWrite);
    QTextStream csv(&fileCsv);

    //запишем поля заголовков в файл
    QSqlRecord fields = db.record(table);

    QStringList str;
    for (int i = 0; i < fields.count(); i++)
    {
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
}

void Converter::convertToSQL()
{
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

        //QString table = ui->tableBox->currentText();
        QString table = name;
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
}
