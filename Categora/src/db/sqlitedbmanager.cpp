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

bool SqliteDbManager::saveToken(QtTokenWrapper &token)
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

    // Get last inserted ID
    const quint64 urlId = query.lastInsertId().toULongLong();
    token.setId(urlId);

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
        QDateTime(QDateTime::fromSecsSinceEpoch(query.value(1).toLongLong())),
        this
    );

    token->setId(id);

    return true;
}

bool SqliteDbManager::readToken(const QString &tokenString, QtTokenWrapper *&token)
{
    QSqlQuery query;
    query.prepare("SELECT id, token, creationDate FROM tokens WHERE token = :tokenString");
    query.bindValue(":tokenString", tokenString);

    if (!query.exec() || !query.next()) {
        qWarning() << "Error reading token: " << query.lastError();
        return false;
    }

    token = new QtTokenWrapper(
        query.value(1).toString(),
        QDateTime(QDateTime::fromSecsSinceEpoch(query.value(2).toLongLong()))
    );

    token->setId(query.value(0).toInt());

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

bool SqliteDbManager::saveUrl(QtUrlWrapper &url)
{
    qDebug() << "Storing url object: " << url;
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
        url.setId(urlId);

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

bool SqliteDbManager::readUrl(quint32 id, QtUrlWrapper *&url)
{
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
    url->setId(id);

    return true;
}

bool SqliteDbManager::readUrl(const QString &urlString, QtUrlWrapper *&url)
{
    QSqlQuery query;
    query.prepare(
        "SELECT id, url, note, createdDate FROM urls "
        "WHERE url = :urlString"
        );
    query.bindValue(":urlString", urlString);

    if (!query.exec() || !query.next()) {
        qWarning() << "URL read error:" << query.lastError();
        return false;
    }

    // Get main URL data
    quint32 urlId = query.value(0).toInt();
    QString urlStr = query.value(1).toString();
    QString note = query.value(2).toString();

    // Get tags
    QSqlQuery tagQuery;
    tagQuery.prepare(
        "SELECT tag FROM url_tags "
        "WHERE url_id = :id"
        );
    tagQuery.bindValue(":id", urlId);

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
    url->setId(urlId);

    return true;
}

bool SqliteDbManager::readUrlFromToken(const QString &token, QList<QtUrlWrapper *> &urlList)
{
    QSqlQuery query;

    // Step 1: Find the token_id for the given token
    query.prepare("SELECT id FROM tokens WHERE token = :token");
    query.bindValue(":token", token);

    if (!query.exec()) {
        qWarning() << "Error finding token_id:" << query.lastError();
        return false;
    }

    if (!query.next()) {
        qWarning() << "No token found with value:" << token;
        return false;
    }

    int tokenId = query.value(0).toInt(); // Get the token_id

    // Step 2: Find all url_ids associated with the token_id
    query.prepare(
        "SELECT url_id FROM token_url "
        "WHERE token_id = :token_id"
        );
    query.bindValue(":token_id", tokenId);

    if (!query.exec()) {
        qWarning() << "Error finding url_ids:" << query.lastError();
        return false;
    }

    QList<int> urlIds;
    while (query.next()) {
        urlIds.append(query.value(0).toInt()); // Collect all url_ids
    }

    if (urlIds.isEmpty()) {
        qWarning() << "No URLs found for token:" << token;
        return false;
    }

    // Step 3: Retrieve URLs and their tags
    for (int urlId : urlIds) {
        QtUrlWrapper *urlWrapper;

        if (!readUrl(urlId, urlWrapper)) {
            break;
        }

        urlList.append(urlWrapper);
    }

    return true;
}

bool SqliteDbManager::updateUrl(quint32 id, const QtUrlWrapper &url)
{
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

bool SqliteDbManager::deleteUrl(quint32 id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM urls WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "Delete URL failed:" << query.lastError();
        return false;
    }

    return true;
}

bool SqliteDbManager::addUrlOwner(const QtTokenWrapper &owner, const QtUrlWrapper &url)
{
    QSqlQuery query;
    query.prepare(
        "INSERT OR IGNORE INTO token_url (token_id, url_id) "
        "VALUES (:tokenid, :urlid)"
        );

    query.bindValue(":tokenid", owner.getId());
    query.bindValue(":urlid", url.getId());

    if (!query.exec()) {
        qWarning() << "Token ownership of url failed";
        return false;
    }

    return true;
}

bool SqliteDbManager::removeUrlOwner(const QtTokenWrapper &owner, const QtUrlWrapper &url)
{
    QSqlQuery query;
    query.prepare(
        "DELETE FROM token_url "
        "WHERE token_id = :tokenid AND url_id = :urlid"
        );

    query.bindValue(":tokenid", owner.getId());
    query.bindValue(":urlid", url.getId());

    if (!query.exec()) {
        qWarning() << "Token ownership removal failed:" << query.lastError().text();
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
    return createTokenTable()
           && createTokenUrlJunction()
           && createUrlTable()
           && createUrlTagsTable();
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

bool SqliteDbManager::createTokenUrlJunction()
{
    QString createTableQuery =
        "CREATE TABLE IF NOT EXISTS token_url ("
        "  token_id INTEGER NOT NULL,"
        "  url_id INTEGER NOT NULL,"
        "  FOREIGN KEY(token_id) REFERENCES tokens(id) ON DELETE CASCADE,"
        "  FOREIGN KEY(url_id) REFERENCES urls(id) ON DELETE CASCADE,"
        "  PRIMARY KEY(token_id, url_id)"
        ")";
    return execQuery(createTableQuery, "token_url");
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
