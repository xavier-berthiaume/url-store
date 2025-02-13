#include "tcpserver.h"

#include <QTcpSocket>
#include <QUuid>

TcpServer::TcpServer(QObject *parent)
    : QTcpServer(parent)
    , AbstractServer()
{}

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
    socket = nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &TcpServer::handleReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &TcpServer::handleDisconnected);
}

void TcpServer::handleReadyRead()
{
    socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QString request = QString::fromUtf8(socket->readAll()).trimmed();
    handleRequest(request, [this](const QString &response) {
        socket->write(response.toUtf8());
    });
}

void TcpServer::handleDisconnected()
{
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (socket) socket->deleteLater();
}
