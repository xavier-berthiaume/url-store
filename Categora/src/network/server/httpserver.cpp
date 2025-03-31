#include "httpserver.h"

#include <QDebug>
#include <utility>

HttpServer::HttpServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent)
    : AbstractServer(m_db, m_api, parent),
    server(new QHttpServer(this)),
    tcp_server(new QTcpServer(this))
{
    setupRoutes();
}

HttpServer::~HttpServer()
{
    stopServer();
}

void HttpServer::startServer(quint16 port)
{
    if (!tcp_server->listen(QHostAddress::Any, port)) {
        qCritical() << "Failed to start server on port" << port;
        exit(1);
    }

    if (!server->bind(tcp_server)) {
        qCritical() << "Failed to bind HTTP server to TCP";
        exit(1);
    }

    qDebug() << "Server listening on port" << port;
}

void HttpServer::stopServer()
{
    tcp_server->close();
}

QString HttpServer::getAuthToken(const QHttpServerRequest &request) {
    qDebug() << "Getting authorization header of request";

    QByteArrayView authHeader = request.headers().value(QHttpHeaders::WellKnownHeader::Authorization);

    if (authHeader.isEmpty()) {
        qWarning() << "No Authorization header found";
        return QString();
    }

    const QByteArray authHeaderBytes = authHeader.toByteArray();

    if (!authHeaderBytes.startsWith("Bearer ")) {
        qWarning() << "Invalid Authorization format (expected Bearer token)";
        return QString();
    }

    QString token = QString::fromUtf8(
                        authHeaderBytes.constData() + 7,  // Skip "Bearer "
                        authHeaderBytes.size() - 7
                        ).trimmed();

    return token;
}

QString HttpServer::getUrlFromQuery(const QHttpServerRequest &request) {
    QUrlQuery query(request.url().query());

    QString urlParam = query.queryItemValue("url", QUrl::FullyDecoded);

    if (urlParam.isEmpty()) {
        qWarning() << "Missing 'url' query parameter";
        return QString();
    }

    return urlParam;
}

QHttpServerResponse HttpServer::addCorsHeaders(QHttpServerResponse &&response)
{
    QHttpHeaders headers;
    headers.append("Access-Control-Allow-Origin", "*");
    headers.append("Access-Control-Allow-Methods", "GET, POST, DELETE, OPTIONS");
    headers.append("Access-Control-Allow-Headers", "Content-Type, Authorization");

    response.setHeaders(headers);

    return std::move(response);
}

void HttpServer::setupRoutes()
{
    setupAuthRoute();
    setupUrlPostRoute();
    setupUrlDeleteRoute();
    setupUrlGetRoute();
}

void HttpServer::setupAuthRoute()
{
    server->route("/auth", QHttpServerRequest::Method::Post, [this]() {
        QHttpServerResponse response = QHttpServerResponse(this->handleAuth(), QHttpServerResponse::StatusCode::Ok);
        return addCorsHeaders(std::move(response));
    });
}

void HttpServer::setupUrlPostRoute()
{
    server->route("/url", QHttpServerRequest::Method::Post, [this](const QHttpServerRequest &request) {
        const QString token = this->getAuthToken(request);

        if (token.isEmpty()) {
            return QHttpServerResponse(QHttpServerResponse::StatusCode::Forbidden);
        }

        const QString url = this->getUrlFromQuery(request);

        if (url.isEmpty()) {
            return QHttpServerResponse(QString("Missing 'url' query parameter"), QHttpServerResponse::StatusCode::ExpectationFailed);
        }

        return QHttpServerResponse(this->handlePost(token, url), QHttpServerResponse::StatusCode::Accepted);
    });
}

void HttpServer::setupUrlDeleteRoute()
{
    server->route("/url", QHttpServerRequest::Method::Delete, [this](const QHttpServerRequest &request) {
        const QString token = this->getAuthToken(request);

        if (token.isEmpty()) {
            return QHttpServerResponse(QHttpServerResponse::StatusCode::Forbidden);
        }

        const QString url = this->getUrlFromQuery(request);

        if (url.isEmpty()) {
            return QHttpServerResponse(QString("Missing 'url' query parameter"), QHttpServerResponse::StatusCode::ExpectationFailed);
        }

        return QHttpServerResponse(this->handleDelete(token, url), QHttpServerResponse::StatusCode::Ok);
    });
}

void HttpServer::setupUrlGetRoute()
{
    server->route("/url", QHttpServerRequest::Method::Get, [this](const QHttpServerRequest &request) {
        const QString token = this->getAuthToken(request);

        if (token.isEmpty()) {
            return QHttpServerResponse(QHttpServerResponse::StatusCode::Forbidden);
        }

        return QHttpServerResponse(this->handleGet(token), QHttpServerResponse::StatusCode::Ok);
    });
}
