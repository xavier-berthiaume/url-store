#include "tcpserver.h"

#include <QTcpSocket>
#include <QUuid>

TcpServer::TcpServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent)
    : AbstractServer(m_db, m_api, parent)
{
    server = new QTcpServer(this);
    qDebug() << "Connecting TcpServer new connection handler:"
             << connect(server, &QTcpServer::newConnection, this, &TcpServer::handleNewConnection);
}

void TcpServer::startServer(quint16 port)
{
    if (!server->listen(QHostAddress::Any, port)) {
        qCritical() << "Failed to start server on port" << port;
        exit(1);
    }

    qDebug() << "Server listening on port" << port;
}

void TcpServer::stopServer() {
    server->close();
    qDebug() << "TCP Server stopped.";
}


void TcpServer::handleNewConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    // activeSockets.append(socket);

    connect(socket, &QTcpSocket::readyRead, this, &TcpServer::handleReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TcpServer::handleDisconnected);
    qDebug() << "New client connected: " << socket->peerAddress().toString();
}

void TcpServer::handleReadyRead()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString request = QString::fromUtf8(socket->readAll()).trimmed();

    handleRequest(request, [socket](const QString &response) {
        qDebug() << "Sending response: " << response;
        socket->write(response.toUtf8());
        socket->flush();
    });
}

void TcpServer::handleDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) {
        activeSockets.removeOne(socket);
        socket->deleteLater();
    }
}
