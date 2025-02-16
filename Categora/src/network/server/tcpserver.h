#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

#include "abstractserver.h"

class TcpServer : public QTcpServer, public AbstractServer
{
    Q_OBJECT

    QList<QTcpSocket*> activeSockets;

public:
    explicit TcpServer(QObject *parent = nullptr);
    void startServer(quint16 port) override;
    void stopServer() override;

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();

};

#endif
