#ifndef APIMANAGER_H
#define APIMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

// Abstract class for the LLM API
class ApiManager : public QObject
{
    Q_OBJECT

protected:
    enum RequestType {
        FetchUrlPage,
        FetchTags,
    };

    QNetworkAccessManager *networkManager;
    QHash<QNetworkReply *, RequestType> requestTypeMap;
    QString apiKey;

public:
    explicit ApiManager(QObject *parent = nullptr);

    virtual void setApiKey(const QString &apiKey) = 0;

    virtual void fetchTags(const QString &url) = 0;
    virtual void fetchPageContent(const QString &url) = 0;

    virtual void cancelRequest() = 0;

signals:
    void tagsFetched(const QStringList &tags);
    void pageFetched(const QString &content);
    void errorFetchingData(const QString &error);


};

#endif // APIMANAGER_H
