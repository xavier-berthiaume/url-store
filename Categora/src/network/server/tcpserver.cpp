#include "tcpserver.h"

#include <QTcpSocket>
#include <QUuid>

TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
    , AbstractServer()
{
    qDebug() << "Connecting TcpServer new connection handler:"
             << connect(this, &QTcpServer::newConnection, this, &TcpServer::handleNewConnection);
}

void TcpServer::startServer(quint16 port)
{
    if (!listen(QHostAddress::Any, port)) {
        qCritical() << "Failed to start server on port" << port;
        exit(1);
    }

    qDebug() << "Server listening on port" << port;
}

void TcpServer::stopServer() {
    close();
    qDebug() << "TCP Server stopped.";
}


void TcpServer::handleNewConnection()
{
    QTcpSocket *socket = nextPendingConnection();
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
