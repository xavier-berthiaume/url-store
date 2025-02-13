#ifndef MOCKNETWORKACCESSMANAGER_H
#define MOCKNETWORKACCESSMANAGER_H

#include "mocknetworkreply.h"

#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class MockNetworkAccessManager : public QNetworkAccessManager {
    Q_OBJECT

public:
    MockNetworkAccessManager(QObject* parent = nullptr)
        : QNetworkAccessManager(parent) {}

    QNetworkReply* createRequest(
        Operation op,
        const QNetworkRequest& request,
        QIODevice* outgoingData = nullptr
        ) override {
        // Capture the request and data
        lastRequest = request;
        if (outgoingData) {
            lastData = outgoingData->readAll(); // Read the outgoing payload
        }

        // Return a mock reply
        return new MockNetworkReply(this);
    }

    QNetworkRequest lastRequest;
    QByteArray lastData;
};
#endif
