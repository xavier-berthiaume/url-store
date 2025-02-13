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
    MockNetworkAccessManager(QObject* parent = nullptr) : QNetworkAccessManager(parent) {}

    QNetworkReply* post(const QNetworkRequest& request, const QByteArray& data) {
        lastRequest = request;
        lastData = data;

        // Create a mock reply
        QNetworkReply* reply = new MockNetworkReply(this);
        return reply;
    }

    QNetworkRequest lastRequest;
    QByteArray lastData;
};

#endif
