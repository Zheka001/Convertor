#include <processing.h>

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

            ret.append(withoutQuotes(sub_str));
            str.remove(0,sub_str.size()+1);
        }

        ret.append(withoutQuotes(str));
        return ret;
}

QString withoutQuotes(QString str)
{
    if (str[0] == "\"")
    {
        str.remove(0,1);
        str.remove(str.size()-1,1);
    }

    str.replace("\"\"","\"");

    return str;
}

QString processingForCsvStr(QString str)
{
    if (str.contains(";") || str.contains(",") || str.contains("\"") || str.contains("\n"))
        return "\"" + str + "\"";

    if (str.contains("\""))
    {
        str.replace("\"","\"\"");
    }
    return str;
}
