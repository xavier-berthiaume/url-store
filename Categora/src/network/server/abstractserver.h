#ifndef ABSTRACTSERVER_H
#define ABSTRACTSERVER_H

#include <QMap>

#include "db/abstractdbmanager.h"

class AbstractServer
{

protected:
    AbstractDbManager *m_db;
    QMap<QString, QString> m_tokens;
    QMap<QString, QStringList> m_url_store;

    QString generateToken();
    bool isValidToken(const QString &token);
    void handleRequest(const QString &request, std::function<void(const QString &)> sendResponse);

public:
    AbstractServer(AbstractDbManager *manager = nullptr);

    virtual void startServer(quint16 port) = 0;
    virtual void stopServer() = 0;
};

#endif // ABSTRACTSERVER_H
