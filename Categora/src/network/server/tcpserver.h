#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

#include "abstractserver.h"

class TcpServer : public QTcpServer, public AbstractServer
{
    Q_OBJECT

    QTcpSocket *socket;

public:
    explicit TcpServer(QObject *parent = nullptr);
    void startServer(quint16 port) override;
    void stopServer() override;

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();

};

#endif // SERVER_H
