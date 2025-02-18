#include "sqlitedbmanager.h"

#include <QDateTime>
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

bool SqliteDbManager::saveToken(const QtTokenWrapper &token)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO tokens (token, creationDate) "
        "VALUES (:token, :date)"
        );
    query.bindValue(":token", token.tokenString());
    query.bindValue(":date", token.creationDate().toSecsSinceEpoch());

    if (!query.exec()) {
        qWarning() << "Error saving token:" << query.lastError();
        return false;
    }

    return true;
}

bool SqliteDbManager::readToken(quint32 id, QtTokenWrapper *&token)
{
    QSqlQuery query;
    query.prepare("SELECT token, creationDate FROM tokens WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        qWarning() << "Error reading token:" << query.lastError();
        return false;
    }

    token = new QtTokenWrapper(
        query.value(0).toString(),
        query.value(1).toDateTime(),
        this
    );

    return true;
}

bool SqliteDbManager::updateToken(quint32 id, const QtTokenWrapper &token)
{
    QSqlQuery query;
    query.prepare("UPDATE tokens SET token=:tokenStr, creationDate=:creationDate WHERE id=:id");
    query.bindValue(":tokenStr", token.tokenString());
    query.bindValue(":creationDate", token.creationDate());
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Error updating token: " << query.lastError();
        return false;
    }

    return true;
}

bool SqliteDbManager::deleteToken(quint32 id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM tokens WHERE id=:id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Error deleting token " << query.lastError();
        return false;
    }

    return true;
}

bool SqliteDbManager::saveUrl(const QtUrlWrapper &url) {
    QSqlDatabase::database().transaction(); // Start transaction

    try {
        // Insert main URL data
        QSqlQuery query;
        query.prepare(
            "INSERT INTO urls (url, note, createdDate) "
            "VALUES (:url, :note, :date)"
            );
        query.bindValue(":url", url.url());
        query.bindValue(":note", url.note());
        query.bindValue(":date", QDateTime::currentSecsSinceEpoch());

        if (!query.exec()) throw std::runtime_error("URL insert failed");

        // Get last inserted ID
        const quint64 urlId = query.lastInsertId().toULongLong();

        // Insert tags
        for (const auto& tag : url.tags()) {
            QSqlQuery tagQuery;
            tagQuery.prepare(
                "INSERT OR IGNORE INTO url_tags (url_id, tag) "
                "VALUES (:id, :tag)"
                );
            tagQuery.bindValue(":id", urlId);
            tagQuery.bindValue(":tag", tag);

            if (!tagQuery.exec()) throw std::runtime_error("Tag insert failed");
        }

        QSqlDatabase::database().commit();
        return true;
    }
    catch (const std::exception& e) {
        QSqlDatabase::database().rollback();
        qWarning() << "Save URL failed:" << e.what();
        return false;
    }
}

bool SqliteDbManager::readUrl(quint32 id, QtUrlWrapper *&url) {
    QSqlQuery query;
    query.prepare(
        "SELECT url, note, createdDate FROM urls "
        "WHERE id = :id"
        );
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        qWarning() << "URL read error:" << query.lastError();
        return false;
    }

    // Get main URL data
    QString urlStr = query.value(0).toString();
    QString note = query.value(1).toString();

    // Get tags
    QSqlQuery tagQuery;
    tagQuery.prepare(
        "SELECT tag FROM url_tags "
        "WHERE url_id = :id"
        );
    tagQuery.bindValue(":id", id);

    QStringList tags;
    if (tagQuery.exec()) {
        while (tagQuery.next()) {
            tags << tagQuery.value(0).toString();
        }
    }

    // Create URL object
    url = new QtUrlWrapper(urlStr, tags, note, this);
    url->setNote(note);
    url->setTags(tags);

    return true;
}

bool SqliteDbManager::updateUrl(quint32 id, const QtUrlWrapper &url) {
    QSqlDatabase::database().transaction();

    try {
        // Update main URL data
        QSqlQuery query;
        query.prepare(
            "UPDATE urls SET "
            "url = :new_url, note = :new_note "
            "WHERE id = :id"
            );
        query.bindValue(":new_url", url.url());
        query.bindValue(":new_note", url.note());
        query.bindValue(":id", id);

        if (!query.exec()) throw std::runtime_error("URL update failed");

        // Update tags (delete old ones first)
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM url_tags WHERE url_id = :id");
        deleteQuery.bindValue(":id", id);
        if (!deleteQuery.exec()) throw std::runtime_error("Tag delete failed");

        // Insert new tags
        for (const auto& tag : url.tags()) {
            QSqlQuery insertQuery;
            insertQuery.prepare(
                "INSERT OR IGNORE INTO url_tags (url_id, tag) "
                "VALUES (:id, :tag)"
                );
            insertQuery.bindValue(":id", id);
            insertQuery.bindValue(":tag", tag);

            if (!insertQuery.exec()) throw std::runtime_error("Tag insert failed");
        }

        QSqlDatabase::database().commit();
        return true;
    }
    catch (const std::exception& e) {
        QSqlDatabase::database().rollback();
        qWarning() << "Update URL failed:" << e.what();
        return false;
    }
}

bool SqliteDbManager::deleteUrl(quint32 id) {
    QSqlQuery query;
    query.prepare("DELETE FROM urls WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Delete URL failed:" << query.lastError();
        return false;
    }

    return true;
}

void SqliteDbManager::close()
{
    m_database.close();
}

bool SqliteDbManager::createTables()
{
    return createTokenTable() && createUrlTable() && createUrlTagsTable();
}

bool SqliteDbManager::createTokenTable()
{
    QString createTableQuery =
        "CREATE TABLE IF NOT EXISTS tokens ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  token TEXT NOT NULL UNIQUE,"
        "  creationDate INTEGER NOT NULL"
        ")";
    return execQuery(createTableQuery, "token");
}

bool SqliteDbManager::createUrlTable()
{
    QString createTableQuery =
        "CREATE TABLE IF NOT EXISTS urls ("
        "  id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "  url TEXT NOT NULL UNIQUE,"
        "  note TEXT,"
        "  createdDate INTEGER NOT NULL"
        ")";
    return execQuery(createTableQuery, "url");
}

bool SqliteDbManager::createUrlTagsTable()
{
    QString createTableQuery =
        "CREATE TABLE IF NOT EXISTS url_tags ("
        "  url_id INTEGER NOT NULL,"
        "  tag TEXT NOT NULL,"
        "  FOREIGN KEY(url_id) REFERENCES urls(id) ON DELETE CASCADE,"
        "  PRIMARY KEY(url_id, tag)"
        ")";
    return execQuery(createTableQuery, "url_tags");
}

// Helper function for executing queries
bool SqliteDbManager::execQuery(const QString& query, const QString& tableName)
{
    QSqlQuery q;
    if (!q.exec(query)) {
        qWarning() << "Error creating " << tableName << " table: " << q.lastError();
        return false;
    }

    return true;
}
