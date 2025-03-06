#include "deepseekapimanager.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

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
    qDebug() << "Fetching tags for url or content: " << urlOrContent;
    QUrl apiUrl(getBaseUrl());
    QNetworkRequest request(apiUrl);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QString("Bearer %1").arg(apiKey).toUtf8());

    QJsonObject payload;
    payload["model"] = "deepseek-chat";
    payload["stream"] = false;

    QJsonArray messages;
    messages.append(QJsonObject {
        {"role", "system"},
        {"content", QString("Generate tags for this link. I require only the tags, comma separated: %1").arg(urlOrContent)},
    });

    payload["messages"] = messages;
    QJsonDocument doc(payload);
    QByteArray data = doc.toJson();

    qDebug() << "Posting the following request with headers " << request.headers();
    qDebug() << "Posting the following request with data " << data;

    currentReply = networkManager->post(request, data);
    requestTypeMap[currentReply] = qMakePair(FetchTags, urlOrContent);
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
    qDebug() << "=== DeepSeek API Response ===";
    qDebug() << "URL:" << reply->url().toString();
    qDebug() << "HTTP status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qDebug() << "Error:" << reply->errorString();
    qDebug() << "Response headers:" << reply->rawHeaderPairs();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorFetchingData(reply->errorString());
        return;
    }

    QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
    QJsonObject responseObj = responseDoc.object();

    qDebug() << "Response object: " << responseObj;

    auto requestInfo = requestTypeMap.value(reply);
    RequestType type = requestInfo.first;
    QString originalUrl = requestInfo.second;

    if (type == FetchTags) {
        if (responseObj.contains("choices")) {
            QJsonArray choices = responseObj["choices"].toArray();
            if (!choices.isEmpty()) {
                QString content = choices[0].toObject()["message"].toObject()["content"].toString();
                qDebug() << "Response content: " << content;
                QStringList tags = content.split(", ");
                emit tagsFetched(originalUrl, tags);
            }
        }
    }

    requestTypeMap.remove(reply);
    reply->deleteLater();
}
