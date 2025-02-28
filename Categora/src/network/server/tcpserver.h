#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>

#include "abstractserver.h"

class TcpServer : public AbstractServer
{
    Q_OBJECT

    QTcpServer *server;
    QList<QTcpSocket*> activeSockets;

public:
    explicit TcpServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent = nullptr);
    void startServer(quint16 port) override;
    void stopServer() override;

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();

};

#endif
