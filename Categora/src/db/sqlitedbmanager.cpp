#include "sqlitedbmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SqliteDbManager::SqliteDbManager(const QString &databasePath, QObject *parent)
    : AbstractDbManager(parent)
{
    m_database_path = databasePath;
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName(m_database_path);
}

bool SqliteDbManager::init()
{
    if (!m_database.open()) {
        qWarning() << "Error: Could not open database.";
        return false;
    }

    if (!createTables()) {
        qWarning() << "Error: Could not create tables.";
        return false;
    }

    return true;
}

bool SqliteDbManager::saveObject(const QObject &object)
{
    // Implement saving logic here
    return true;
}

bool SqliteDbManager::readObject(QObject &object, const QString &id)
{
    // Implement reading logic here
    return true;
}

bool SqliteDbManager::updateObject(const QObject &object)
{
    // Implement update logic here
    return true;
}

bool SqliteDbManager::deleteObject(const QString &id)
{
    // Implement delete logic here
    return true;
}

void SqliteDbManager::close()
{
    m_database.close();
}

bool SqliteDbManager::createTables()
{
    if (!createTokenTable())
        return false;

    if (createUrlTable())
        return false;

    return true;
}

bool SqliteDbManager::createTokenTable()
{
    QSqlQuery query;
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS tokens ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "token TEXT NOT NULL"
                               "creationDate INTEGER NOT NULL)";
    if (!query.exec(createTableQuery)) {
        qWarning() << "Error: Could not create token table -" << query.lastError();
        return false;
    }
    return true;
}

bool SqliteDbManager::createUrlTable()
{
    QSqlQuery query;
    QString createTableQuery = "CREATE TABLE IF NOT EXISTS url ("
                               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                               "tokenid INTEGER"
                               "url TEXT NOT_NULL)";
    if (!query.exec(createTableQuery)) {
        qWarning() << "Error: Could not create url table -" << query.lastError();
        return false;
    }
    return true;
}
