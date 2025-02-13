#include "deepseekapimanager.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DeepSeekApiManager::DeepSeekApiManager(QObject *parent)
    : ApiManager{parent}
{
    networkManager = new QNetworkAccessManager(this);
}

void DeepSeekApiManager::setApiKey(const QString &apiKey) {
    this->apiKey = apiKey;
}



// TODO: IMPLEMENT TO FETCH PAGE CONTENT
void DeepSeekApiManager::fetchPageContent(const QString &url) {
    // Implement fetching page content if needed
    Q_UNUSED(url);
}

void DeepSeekApiManager::cancelRequest() {
    if (currentReply) {
        currentReply->abort();
        currentReply->deleteLater();
        currentReply = nullptr;
    }
}

void DeepSeekApiManager::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        emit errorFetchingData(reply->errorString());
        return;
    }

    QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject responseObj = responseDoc.object();

    if (requestTypeMap[reply] == FetchTags) {
        if (responseObj.contains("choices")) {
            QJsonArray choices = responseObj["choices"].toArray();
            if (!choices.isEmpty()) {
                QString content = choices[0].toObject()["message"].toObject()["content"].toString();
                QStringList tags = content.split(": ").last().split(", ");
                emit tagsFetched(tags);
            }
        }
    }

    requestTypeMap.remove(reply);
    reply->deleteLater();
}
