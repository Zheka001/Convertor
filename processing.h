#ifndef PROCESSING_H
#define PROCESSING_H

#include <QString>
#include <QStringList>

QString processingForCsvStr(QString str);

QString whatType(QString str);

QStringList parseStr(QString str);

QString withoutQuotes(QString str);

#endif // PROCESSING_H
