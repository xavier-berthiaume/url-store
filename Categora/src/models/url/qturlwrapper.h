#ifndef QTURLWRAPPER_H
#define QTURLWRAPPER_H

#include <QObject>
#include <QString>
#include <QStringList>

#include "url.h"

class QtUrlWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags NOTIFY tagsChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)

    int id;

public:
    explicit QtUrlWrapper(QObject *parent = nullptr);
    explicit QtUrlWrapper(const Url& url, QObject *parent = nullptr);
    explicit QtUrlWrapper(const QString& url, QObject *parent = nullptr);
    explicit QtUrlWrapper(const QString& url, const QStringList &tags, const QString &note, QObject *parent = nullptr);

    // Conversion to/from core Url object
    Url coreUrl() const;
    void setCoreUrl(const Url& url);

    // Q_PROPERTY accessors
    QString url() const;
    QStringList tags() const;
    QString note() const;

    int getId() const;

    void setId(int id);

    // Database interaction helpers
    QVariantMap toVariantMap() const;
    static QtUrlWrapper* fromVariantMap(const QVariantMap& data, QObject* parent = nullptr);

    bool operator==(const QtUrlWrapper &other) const;
    bool operator!=(const QtUrlWrapper &other) const;
    // TODO: Review the next function for string output of objects
    friend QDebug operator<<(QDebug debug, const QtUrlWrapper &url);

public slots:
    void setUrl(const QString& url);
    void setTags(const QStringList& tags);
    void setNote(const QString& note);

signals:
    void urlChanged(const QString& newUrl);
    void tagsChanged(const QStringList& newTags);
    void noteChanged(const QString& newNote);

private:
    Url m_url;

};

#endif // QTURLWRAPPER_H
