#include "apimanager.h"

ApiManager::ApiManager(QObject *parent)
    : QObject{parent}
{}

void ApiManager::setApiKey(const QString& apiKey)
{
    this->apiKey = apiKey;
}

void ApiManager::setNetworkManager(QNetworkAccessManager *manager)
{
    this->networkManager = manager;
}

QString ApiManager::getApiKey() const
{
    return apiKey;
}
