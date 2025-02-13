#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QObject>

class DbManager : public QObject
{
    Q_OBJECT
public:
    explicit DbManager(QObject *parent = nullptr);

signals:
};

#endif // DBMANAGER_H
