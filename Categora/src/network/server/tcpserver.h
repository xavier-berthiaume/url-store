#ifndef SERVER_H
#define SERVER_H

#include "abstractserver.h"

#include <QTcpServer>

class TcpServer : public AbstractServer, public QTcpServer
{
    Q_OBJECT

    QTcpSocket *socket;

public:
    explicit TcpServer(QObject *parent = nullptr);
    void startServer(quint16 port);
    void stopServer();

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();

};

#endif // SERVER_H
