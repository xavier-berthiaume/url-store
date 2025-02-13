#include "../../src/api/deepseek/deepseekapimanager.h"

#include "mocknetworkaccessmanager.h"

#include <QObject>
#include <QtTest/QTest>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class DeepSeekApiManagerTest : public QObject {
    Q_OBJECT

private slots:
    void testFetchTags() {
        // Create the DeepSeekApiManager instance
        DeepSeekApiManager apiManager;
        apiManager.setApiKey("testapikey");

        // Replace the network manager with a mock
        MockNetworkAccessManager* mockNetworkManager = new MockNetworkAccessManager(this);
        apiManager.setNetworkManager(mockNetworkManager);

        // Call the method to test
        QString testUrl = "https://example.com";
        apiManager.fetchTags(testUrl);

        // Verify the request configuration
        QNetworkRequest request = mockNetworkManager->lastRequest;

        qDebug() << "Request URL:" << request.url().toString();
        qDebug() << "Headers:" << request.rawHeaderList();
        qDebug() << "Content-Type:" << request.header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "Auth Header:" << request.rawHeader("Authorization");
        qDebug() << "Payload Data:" << mockNetworkManager->lastData;

        QCOMPARE(request.url().toString(), apiManager.getBaseUrl());
        QCOMPARE(request.header(QNetworkRequest::ContentTypeHeader).toString(), "application/json");
        QCOMPARE(request.rawHeader("Authorization"), QString("Bearer %1").arg(apiManager.getApiKey()).toUtf8());

        // Verify the payload
        QJsonDocument doc = QJsonDocument::fromJson(mockNetworkManager->lastData);
        QJsonObject payload = doc.object();
        QCOMPARE(payload["model"].toString(), QString("deepseek-chat"));

        QJsonArray messages = payload["messages"].toArray();
        QCOMPARE(messages.size(), 1);

        QJsonObject message = messages[0].toObject();
        QCOMPARE(message["role"].toString(), QString("system"));
        QCOMPARE(message["content"].toString(), QString("Generate tags for this link: %1").arg(testUrl));
    }

public:
    explicit DeepSeekApiManagerTest(QObject *parent = nullptr)
        : QObject{parent} {}
};

QTEST_MAIN(DeepSeekApiManagerTest)
#include "deepseekapitests.moc"
