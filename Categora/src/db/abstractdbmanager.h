#ifndef ABSTRACTDBMANAGER_H
#define ABSTRACTDBMANAGER_H

#include <QObject>

#include "../models/url/qturlwrapper.h"
#include "../models/token/qttokenwrapper.h"

class AbstractDbManager : public QObject
{
    Q_OBJECT

public:
    explicit AbstractDbManager(QObject *parent = nullptr);

    virtual bool init() = 0;

    virtual bool saveToken(const QtTokenWrapper &token) = 0;
    virtual bool readToken(quint32 id, QtTokenWrapper *&token) = 0;
    virtual bool readToken(const QString &tokenString, QtTokenWrapper *&token) = 0;
    virtual bool updateToken(quint32 id, const QtTokenWrapper &token) = 0;
    virtual bool deleteToken(quint32 id) = 0;

    virtual bool saveUrl(const QtUrlWrapper &url) = 0;
    virtual bool readUrl(quint32 id, QtUrlWrapper *&url) = 0;
    virtual bool readUrl(const QString &urlString, QtUrlWrapper *&url) = 0;
    virtual bool readUrlFromToken(const QString &tokenString, QList<QtUrlWrapper *> &urlList);
    virtual bool updateUrl(quint32 id, const QtUrlWrapper &url) = 0;
    virtual bool deleteUrl(quint32 id) = 0;

    virtual bool addUrlOwner(const QtTokenWrapper &owner, const QtUrlWrapper &url) = 0;
    virtual bool removeUrlOwner(const QtTokenWrapper &owner, const QtUrlWrapper &url) = 0;

    virtual void close() = 0;

protected:
    virtual bool createTables() = 0;

    QString m_database_path;
};

#endif // ABSTRACTDBMANAGER_H
