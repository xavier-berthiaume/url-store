#ifndef ABSTRACTDBMANAGER_H
#define ABSTRACTDBMANAGER_H

#include <QObject>

#include "../models/url/url.h"
#include "../models/token/token.h"

class AbstractDbManager : public QObject
{
    Q_OBJECT

public:
    explicit AbstractDbManager(QObject *parent = nullptr);

    virtual bool init() = 0;

    virtual bool saveToken(const Token &token) = 0;
    virtual bool readToken(Token &token, quint32 id) = 0;
    virtual bool updateToken(const Token &token) = 0;
    virtual bool deleteToken(quint32 &id) = 0;

    virtual bool saveUrl(const Url &url) = 0;
    virtual bool readUrl(Url &url, quint32 id) = 0;
    virtual bool updateUrl(const Url &url) = 0;
    virtual bool deleteUrl(quint32 id) = 0;

    virtual void close() = 0;

protected:
    virtual bool createTables() = 0;

    QString m_database_path;
};

#endif // ABSTRACTDBMANAGER_H
