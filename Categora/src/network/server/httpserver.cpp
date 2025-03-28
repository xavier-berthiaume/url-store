#include "httpserver.h"
#include <QDebug>

HttpServer::HttpServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent)
    : AbstractServer(m_db, m_api, parent),
    server(new QHttpServer(this))
{
    setupRoutes();
}

HttpServer::~HttpServer()
{
    stopServer();
}

void HttpServer::startServer(quint16 port)
{
    const auto actualPort = server->listen(QHostAddress::Any, port);
    if (!actualPort) {
        qCritical() << "Failed to start HTTP server on port" << port;
        exit(1);
    }
    qInfo() << "HTTP Server listening on port" << actualPort;
}

void HttpServer::stopServer()
{
    if (server) {
        server->close();
        qInfo() << "HTTP Server stopped";
    }
}

void HttpServer::setupRoutes()
{
    // Example route
    server->route("/", []() {
        return "Hello from QHttpServer!";
    });

    // Your existing request handling
    server->route("/api", QHttpServerRequest::Method::Post,
                  [this](const QHttpServerRequest &request) {
                      const QString requestBody = QString::fromUtf8(request.body());

                      // This assumes handleRequest is from your AbstractServer
                      return QHttpServerResponse([this, requestBody](auto &&responder) {
                          handleRequest(requestBody, [responder](const QString &response) mutable {
                              responder.sendResponse(response.toUtf8(), "application/json");
                          });
                      });
                  });
}
