#include "deepseekapimanager.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

DeepSeekApiManager::DeepSeekApiManager(QObject *parent)
    : ApiManager{parent}
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &DeepSeekApiManager::onReplyFinished);
}

const QString& DeepSeekApiManager::getBaseUrl() const
{
    static const QString kBASEURL = "https://api.deepseek.com/chat/completions";
    return kBASEURL;
}

void DeepSeekApiManager::setApiKey(const QString &apiKey)
{
    this->apiKey = apiKey;
}

void DeepSeekApiManager::fetchTags(const QString &urlOrContent)
{
    QUrl apiUrl(getBaseUrl());
    QNetworkRequest request(apiUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());

    QJsonObject payload;
    payload["model"] = "deepseek-chat";

    QJsonArray messages;
    messages.append(QJsonObject {
        {"role", "system"},
        {"content", QString("Generate tags for this link: %1").arg(urlOrContent)},
    });

    payload["messages"] = messages;
    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    currentReply = networkManager->post(request, data);
    requestTypeMap[currentReply] = FetchTags;
}

// TODO: IMPLEMENT TO FETCH PAGE CONTENT
void DeepSeekApiManager::fetchPageContent(const QString &url)
{
    // Implement fetching page content if needed
    Q_UNUSED(url);
}

void DeepSeekApiManager::cancelRequest()
{
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
