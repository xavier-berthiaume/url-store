#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>

class ApiManager : public QObject
{
    Q_OBJECT
public:
    explicit ApiManager(QObject *parent = nullptr);

signals:
};

#endif // APIMANAGER_H
