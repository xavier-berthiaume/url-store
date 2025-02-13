#ifndef DEEPSEEKAPIMANAGER_H
#define DEEPSEEKAPIMANAGER_H

#include "../apimanager.h"

#include <QNetworkReply>

class DeepSeekApiManager : public ApiManager
{
    Q_OBJECT

    QNetworkReply *currentReply;
    const QString& getBaseUrl() const;

public:
    explicit DeepSeekApiManager(QObject *parent = nullptr);

    void setApiKey(const QString &apiKey) override;
    void fetchTags(const QString &urlOrContent) override;
    void fetchPageContent(const QString &url) override;
    void cancelRequest() override;

private slots:
    void onReplyFinished(QNetworkReply *reply);
};

#endif // DEEPSEEKAPIMANAGER_H
