#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include "abstractdbmanager.h"

#include <QSqlDatabase>

class SqliteDbManager : public AbstractDbManager
{
    QSqlDatabase m_database;

    bool createTokenTable();
    bool createUrlTable();

public:
    explicit SqliteDbManager(const QString &databasePath, QObject *parent = nullptr);

    bool init() override;
    bool saveObject(const QObject &object) override;
    bool readObject(QObject &object, const QString &id) override;
    bool updateObject(const QObject &object) override;
    bool deleteObject(const QString &id) override;
    void close() override;

protected:
    bool createTables() override;

};

#endif // SQLITEDBMANAGER_H
