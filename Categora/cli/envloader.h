#ifndef ENVLOADER_H
#define ENVLOADER_H

#include <QFile>
#include <QTextStream>
#include <QMap>
#include <QCoreApplication>

class EnvLoader
{
public:
    static QMap<QString, QString> loadEnv(const QString& path = "");

};

#endif // ENVLOADER_H
