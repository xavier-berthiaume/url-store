#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include "abstractdbmanager.h"

#include <QSqlDatabase>

class SqliteDbManager : public AbstractDbManager
{
    QSqlDatabase m_database;

    bool createTokenTable();
    bool createUrlTable();
    bool createUrlTagsTable();
    bool execQuery(const QString &query, const QString &tableName);

public:
    explicit SqliteDbManager(const QString &databasePath, QObject *parent = nullptr);

    bool init() override;

    bool saveToken(const QtTokenWrapper &token) override;
    bool readToken(quint32 id, QtTokenWrapper *token) override;
    bool updateToken(quint32 id, const QtTokenWrapper &token) override;
    bool deleteToken(quint32 id) override;

    bool saveUrl(const QtUrlWrapper &url) override;
    bool readUrl(quint32 id, QtUrlWrapper *url) override;
    bool updateUrl(quint32 id, const QtUrlWrapper &url) override;
    bool deleteUrl(quint32 id) override;

    void close() override;

protected:
    bool createTables() override;

};

#endif // SQLITEDBMANAGER_H
