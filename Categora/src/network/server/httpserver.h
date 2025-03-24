#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include "abstractserver.h"

class HttpServer : public AbstractServer
{
    Q_OBJECT
public:
    explicit HttpServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent = nullptr);

    void startServer(quint16 port) override;
    void stopServer() override;
};

#endif // HTTPSERVER_H
