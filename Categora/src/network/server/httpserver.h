#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QTcpServer>
#include <QtHttpServer/QHttpServer>

#include "abstractserver.h"

class HttpServer : public AbstractServer
{
    Q_OBJECT

    QHttpServer *server;
    QTcpServer *tcp_server;

    QString getAuthToken(const QHttpServerRequest &request);
    QString getUrlFromQuery(const QHttpServerRequest &request);

    QHttpServerResponse addCorsHeaders(QHttpServerResponse &&response);

    void setupRoutes();
    void setupAuthRoute();
    void setupUrlPostRoute();
    void setupUrlDeleteRoute();
    void setupUrlGetRoute();

public:
    explicit HttpServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent = nullptr);
    ~HttpServer();

    void startServer(quint16 port) override;
    void stopServer() override;

};

#endif // HTTPSERVER_H
