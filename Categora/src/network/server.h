#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class Server : public QTcpServer
{
    Q_OBJECT

    QMap<QString, QString> m_tokens;
    QMap<QString, QStringList> m_url_store;

    QString generateToken();
    bool isValidToken(const QString &token);
    void handleRequest(QTcpSocket *socket, const QString &request);

public:
    explicit Server(QObject *parent = nullptr);
    void startServer(quint16 port);

private slots:
    void handleNewConnection();
    void handleReadyRead();
    void handleDisconnected();

};

#endif // SERVER_H
