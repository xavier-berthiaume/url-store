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

bool SqliteDbManager::saveToken(const Token &token)
{
    QSqlQuery query;
    query.prepare(
        "INSERT INTO tokens (token, creationDate) "
        "VALUES (:token, :date)"
        );
    query.bindValue(":token", QString::fromStdString(token.getTokenString()));
    query.bindValue(":date", static_cast<qint64>(token.getCreationDate()));

    if (!query.exec()) {
        qWarning() << "Error saving token:" << query.lastError();
        return false;
    }

    return true;
}

bool SqliteDbManager::readToken(quint32 id, std::unique_ptr<Token> &token)
{
    QSqlQuery query;
    query.prepare("SELECT token, creationDate FROM tokens WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        qWarning() << "Error reading token:" << query.lastError();
        token.reset();
        return false;
    }

    token = std::make_unique<Token>(
        query.value(0).toString().toStdString(),
        query.value(1).toLongLong()
        );

    return true;
}

bool SqliteDbManager::updateToken(quint32 id, const Token &token)
{
    QSqlQuery query;
    query.prepare("UPDATE tokens SET token=:tokenStr, creationDate=:creationDate WHERE id=:id");
    query.bindValue(":tokenStr", QString::fromStdString(token.getTokenString()));
    query.bindValue(":creationDate", static_cast<qint64>(token.getCreationDate()));
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

bool SqliteDbManager::saveUrl(const Url &url) {
    QSqlDatabase::database().transaction();

    try {
        // Insert main URL data
        QSqlQuery query;
        query.prepare(
            "INSERT INTO urls (url, note, createdDate) "
            "VALUES (:url, :note, :date)"
            );
        query.bindValue(":url", QString::fromStdString(url.getUrl()));
        query.bindValue(":note", QString::fromStdString(url.getNote()));
        query.bindValue(":date", QDateTime::currentSecsSinceEpoch());

        if (!query.exec()) throw std::runtime_error("URL insert failed");

        // Get last inserted ID
        const quint64 urlId = query.lastInsertId().toULongLong();

        // Insert tags
        for (const auto& tag : url.getTags()) {
            QSqlQuery tagQuery;
            tagQuery.prepare(
                "INSERT OR IGNORE INTO url_tags (url_id, tag) "
                "VALUES (:id, :tag)"
                );
            tagQuery.bindValue(":id", urlId);
            tagQuery.bindValue(":tag", QString::fromStdString(tag));

            if (!tagQuery.exec()) throw std::runtime_error("Tag insert failed");
        }

        QSqlDatabase::database().commit();
        return true;
    } catch (const std::exception& e) {
        QSqlDatabase::database().rollback();
        qWarning() << "Save URL failed:" << e.what();
        return false;
    }
}

bool SqliteDbManager::readUrl(quint32 id, std::unique_ptr<Url> &url) {
    QSqlQuery query;
    query.prepare(
        "SELECT url, note, createdDate FROM urls "
        "WHERE id = :id"
        );
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        qWarning() << "URL read error:" << query.lastError();
        url.reset();
        return false;
    }

    // Get main URL data
    std::string urlStr = query.value(0).toString().toStdString();
    std::string note = query.value(1).toString().toStdString();

    // Get tags
    QSqlQuery tagQuery;
    tagQuery.prepare(
        "SELECT tag FROM url_tags "
        "WHERE url_id = :id"
        );
    tagQuery.bindValue(":id", id);

    std::vector<std::string> tags;
    if (tagQuery.exec()) {
        while (tagQuery.next()) {
            tags.push_back(tagQuery.value(0).toString().toStdString());
        }
    }

    // Create URL object
    url = std::make_unique<Url>(urlStr);
    url->setNote(note);
    url->setTags(tags);

    return true;
}

bool SqliteDbManager::updateUrl(quint32 id, const Url &url) {
    QSqlDatabase::database().transaction();

    try {
        // Update main URL data
        QSqlQuery query;
        query.prepare(
            "UPDATE urls SET "
            "url = :new_url, note = :new_note "
            "WHERE id = :id"
            );
        query.bindValue(":new_url", QString::fromStdString(url.getUrl()));
        query.bindValue(":new_note", QString::fromStdString(url.getNote()));
        query.bindValue(":id", id);

        if (!query.exec()) throw std::runtime_error("URL update failed");

        // Update tags (delete old ones first)
        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM url_tags WHERE url_id = :id");
        deleteQuery.bindValue(":id", id);
        if (!deleteQuery.exec()) throw std::runtime_error("Tag delete failed");

        // Insert new tags
        for (const auto& tag : url.getTags()) {
            QSqlQuery insertQuery;
            insertQuery.prepare(
                "INSERT OR IGNORE INTO url_tags (url_id, tag) "
                "VALUES (:id, :tag)"
                );
            insertQuery.bindValue(":id", id);
            insertQuery.bindValue(":tag", QString::fromStdString(tag));

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
