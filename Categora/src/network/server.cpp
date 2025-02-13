#include "server.h"

#include <QTcpSocket>
#include <QUuid>

Server::Server(QObject *parent)
    : QTcpServer(parent)
{}

QString Server::generateToken()
{
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

bool Server::isValidToken(const QString &token)
{
    return m_tokens.contains(token);
}

void Server::handleRequest(QTcpSocket *socket, const QString &request)
{
    QStringList parts = request.split(" ");
    if (parts.isEmpty()) {
        socket->write("Invalid request.\n");
        return;
    }

    QString method = parts[0].toUpper();
    QString response;

    if (method == "AUTH") {
        QString token = generateToken();
        m_tokens[token] = "client";
        response = "TOKEN " + token + "\n";
    } else if (method == "GET") {
        if (parts.size() < 3 || !isValidToken(parts[1])) {
            response = "ERROR: Invalid token or request format.\n";
        } else {
            QString token = parts[1];
            QString url = parts[2];
            m_url_store[token].append(url);  // Temporary storage
            response = "OK: URL stored.\n";

            // Later: dataManager.saveUrl(token, url);
        }
    } else if (method == "POST") {
        if (parts.size() < 2 || !isValidToken(parts[1])) {
            response = "ERROR: Invalid token.\n";
        } else {
            QString token = parts[1];
            if (m_url_store.contains(token)) {
                response = "URLS: " + m_url_store[token].join(", ") + "\n";
            } else {
                response = "OK: No URLs found.\n";
            }
        }
    } else {
        response = "ERROR: Unsupported method.\n";
    }

    socket->write(response.toUtf8());
    socket->disconnectFromHost();
}

void Server::startServer(quint16 port)
{
    if (!listen(QHostAddress::Any, port)) {
        qCritical() << "Failed to start server on port" << port;
        exit(1);
    }
    qDebug() << "Server listening on port" << port;
}

void Server::handleNewConnection()
{
    QTcpSocket *socket = nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &Server::handleReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &Server::handleDisconnected);
}

void Server::handleReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString request = QString::fromUtf8(socket->readAll()).trimmed();
    handleRequest(socket, request);
}

void Server::handleDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) socket->deleteLater();
}
