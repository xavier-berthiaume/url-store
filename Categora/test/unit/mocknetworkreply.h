#ifndef MOCKNETWORKREPLY_H
#define MOCKNETWORKREPLY_H

#include <QBuffer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

class MockNetworkReply : public QNetworkReply {
    Q_OBJECT

public:
    MockNetworkReply(QObject* parent = nullptr) : QNetworkReply(parent) {
        setOpenMode(QIODevice::ReadOnly);
    }

    void setResponse(const QByteArray& data) {
        buffer.setData(data);
        buffer.open(QIODevice::ReadOnly);
    }

    qint64 readData(char* data, qint64 maxSize) override {
        return buffer.read(data, maxSize);
    }

    void abort() override {

    }

private:
    QBuffer buffer;
};

#endif
