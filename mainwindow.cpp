#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Открываем файл из ресурсов. Вместо данного файла
    // необходимо указывать путь к вашему требуемому файлу
    QFile file("input.csv");
    if ( !file.open(QFile::ReadOnly | QFile::Text) )
    {
        qDebug() << "File not exists";
    } else
    {
        // Создаём поток для извлечения данных из файла
        QTextStream in(&file);
        QString line = in.readLine();
        QString line2 = in.readLine();
        QString *types = new QString[parseStr(line).size()];

        int i = 0;
        for (QString item : parseStr(line2))
        {
            types[i++] = whatType(item);
        }

        i = 0;

        QString ex_cr("create table T1(");
        QString ex_in("insert into T1(");
        QString ex_v(") values(");
        QString* title = new QString[parseStr(line).size()] ;



        for (QString item : parseStr(line))
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

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("table.sqlite");

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
            QSqlRecord schema = db.record("T1");
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
                    q.exec(QLatin1String("DELETE FROM T1;"));
                    qDebug() << "Truncate!";
                }
                else
                {
                    q.exec(QLatin1String("DROP TABLE T1;"));
                    if (!q.exec(ex_cr))
                        qDebug() << q.lastError();
                }
            }
            else
            {
                q.exec(QLatin1String("DROP TABLE T1;"));
                if (!q.exec(ex_cr))
                    qDebug() << q.lastError();
            }
        }



        if (!q.prepare(ex_in))
               qDebug() << q.lastError();
        for (QString item : parseStr(line2))
        {
            q.addBindValue(item);
        }
        q.exec();

        // Считываем данные до конца файла
        while (!in.atEnd())
        {
            line = in.readLine();

            i=0;
            for (QString item : parseStr(line))
            {
                q.addBindValue(item);
            }
            q.exec();
        }
        file.close();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString whatType(QString str)
{
    QRegExp reg("^\\-?\\d+\\.\\d+$");

    if (str.contains(reg))
        return "REAL";

    reg = QRegExp("^\\-?\\d+$");
    if(str.contains(reg))
        return "INTEGER";
    return "TEXT";
}

//сделать удаление лишних кавычек!ЁЁЁЁЁ!!!!!!!!!!!!!!!!!!!!
QStringList parseStr(QString str)
{
        QStringList ret;
        int in;

        while ( (in = str.indexOf(";")) != -1)
        {
            QString sub_str = str;
            sub_str.remove(in,str.size());
            int count_Q = sub_str.count("\"");

            while (count_Q % 2 != 0)
            {
                int in2 = str.indexOf(";",in+1);
                if (in2 == -1)
                {
                    ret.append(str);
                    return ret;
                }
                sub_str = str;
                sub_str.remove(in2,str.size());
                count_Q = sub_str.count("\"");
                in = in2;
            }

            ret.append(withoutQ(sub_str));
            str.remove(0,sub_str.size()+1);
        }

        ret.append(withoutQ(str));
        return ret;
}

QString withoutQ(QString str)
{
    if (str[0] == "\"")
    {
        str.remove(0,1);
        str.remove(str.size()-1,1);
    }

    str.replace("\"\"","\"");

    return str;
}
