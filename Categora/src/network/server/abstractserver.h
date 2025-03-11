#ifndef ABSTRACTSERVER_H
#define ABSTRACTSERVER_H

#include <QObject>

#include "db/abstractdbmanager.h"
#include "api/apimanager.h"

class AbstractServer : public QObject
{
    Q_OBJECT

    AbstractDbManager *m_db;
    ApiManager *m_api;

    QHash<QString, QString> m_pending_url_to_token;
    QMultiHash<QString, QString> m_pending_token_to_url;

    QString handleAuth();
    QString handlePost(const QString &tokenString, const QString &urlString);
    QString handleDelete(const QString &tokenString, const QString &urlString);
    QString handleGet(const QString &tokenString);
    // QString handleGet(const QString &tokenString, const QString &tagSearch);

protected:
    QString generateToken();
    bool isValidToken(const QString &token);
    void handleRequest(const QString &request, std::function<void(const QString &)> sendResponse);

public:
    AbstractServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent = nullptr);

    virtual void startServer(quint16 port) = 0;
    virtual void stopServer() = 0;

public slots:
    void handleTagsFetched(const QString &urlString, const QStringList &tags);
};

#endif // ABSTRACTSERVER_H
