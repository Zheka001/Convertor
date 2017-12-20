#include "convertclass.h"

convertclass::convertclass()
{
}

void convertclass::convertToSql(QString filename)
{
    fname=filename;

    this->determineType();

    QFile file(fname);
    if ( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File not exists";
    }
    else
    {
        QTextStream in(&file);
        QString line = in.readLine();
        this ->generateQuary(line);
        this->determineSchema(line);
        QSqlQuery q;

        if (!q.prepare(ex_in))
               qDebug() << q.lastError();
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
        dbMy.close();
    }

}

void convertclass::determineType()
{
    QFile file(fname);
    if ( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File not exists";
    }
    else
    {
        QTextStream in(&file);
        QString line = in.readLine();
        if (!in.atEnd())
        {
            line = in.readLine();
            QString temp;
            QStringList parse = parseStr(line);
            types = new QString[parse.count()];
            int i = 0;
            for (QString item : parse)
            {
                types[i++] = whatType(item);
            }
            i = 0;
            while (!in.atEnd())
            {
                i = 0;
                line = in.readLine();
                QStringList parse = parseStr(line);
                for (QString item : parse)
                {
                    temp = whatType(item);
                    if (types[i] != temp)
                    {
                        if ((temp == "TEXT") || (temp == "REAL" && types[i] == "INTEGER"))
                            types[i] = temp;
                    }
                    i++;
                }
            }
        }
        file.close();
    }
}
void convertclass::generateQuary (QString str)
{      
        QStringList parse = parseStr(str);
        int i = 0;

        ex_cr = "create table T1(";
        ex_in = "insert into T1(";
        QString ex_v(") values(");

        for (QString item : parse)
        {
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
}
void convertclass::determineSchema (QString str)
{
    dbMy = QSqlDatabase::addDatabase("QSQLITE");
    QString fileName = QFileDialog::getSaveFileName(nullptr," Save File as", "", "Databases files (*.sqlite)");
    QString shortName = fileName.mid(fileName.lastIndexOf("/") + 1);
    dbMy.setDatabaseName(shortName);

    //открываем базу данных
    if (!dbMy.open())
        qDebug() << dbMy.lastError();

    //получаем список таблиц
    QStringList tables = dbMy.tables();

    QSqlQuery q;
    //проверяем, есть ли таблицы в базе
    if (!tables.empty())
    {
        qDebug() << "Already have tables!";
        QSqlRecord schema = dbMy.record(tables.at(0));
        if (schema.count() == parseStr(str).count() )
        {
            int j = 0;
            for (QString item : parseStr(str))
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
}





