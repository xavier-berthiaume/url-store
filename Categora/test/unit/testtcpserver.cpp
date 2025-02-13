#include "../../src/network/server/tcpserver.h"

#include <QObject>
#include <QTcpSocket>
#include <QTest>

class TestTcpServer : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase() {
        // Start the server
        server = new TcpServer(this);
        server->startServer(serverPort);

        // Set up a client socket
        clientSocket = new QTcpSocket(this);
        clientSocket->connectToHost(QHostAddress::LocalHost, serverPort);
        QVERIFY(clientSocket->waitForConnected(5000)); // Wait for connection
    }

    void cleanupTestCase() {
        // Clean up
        clientSocket->disconnectFromHost();
        server->stopServer();
        delete clientSocket;
        delete server;
    }

    void testAuthentication() {
        // Send AUTH request
        QString response = sendRequest("AUTH");
        QVERIFY(response.startsWith("TOKEN ")); // Check if response starts with "TOKEN "
    }

    void testPostRequest() {
        // First, get a token
        QString tokenResponse = sendRequest("AUTH");
        QString token = tokenResponse.split(" ").last();

        // Send POST request with the token
        QString postResponse = sendRequest("POST " + token + " https://example.com");
        QCOMPARE(postResponse, QString("OK: URL stored.\n")); // Check if URL is stored
    }

    void testGetRequest() {
        // First, get a token
        QString tokenResponse = sendRequest("AUTH");
        QString token = tokenResponse.split(" ").last();

        // Send POST request to store a URL
        sendRequest("POST " + token + " https://example.com");

        // Send GET request to retrieve the URL
        QString getResponse = sendRequest("GET " + token);
        QVERIFY(getResponse.contains("https://example.com")); // Check if URL is retrieved
    }

private:
    TcpServer *server;
    QTcpSocket *clientSocket;
    quint16 serverPort = 12345;

    QString sendRequest(const QString &request) {
        clientSocket->write(request.toUtf8());
        clientSocket->waitForBytesWritten(5000);
        clientSocket->waitForReadyRead(5000);
        return QString::fromUtf8(clientSocket->readAll()).trimmed();
    }
};

QTEST_MAIN(TestTcpServer)
#include "testtcpserver.moc"
