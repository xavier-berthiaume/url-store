#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QHttpServer>
#include "abstractserver.h"

class HttpServer : public AbstractServer
{
    Q_OBJECT

    QHttpServer *server;

public:
    explicit HttpServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent = nullptr);
    ~HttpServer();

    void startServer(quint16 port) override;
    void stopServer() override;

private:
    void setupRoutes();
};

#endif // HTTPSERVER_H
