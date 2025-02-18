#include "qturlwrapper.h"

#include <QDebug>
#include <QVariant>

QtUrlWrapper::QtUrlWrapper(QObject *parent)
    : QObject(parent), m_url() {}

QtUrlWrapper::QtUrlWrapper(const Url& url, QObject *parent)
    : QObject(parent), m_url(url) {}

QtUrlWrapper::QtUrlWrapper(const QString& url, QObject *parent)
    : QObject(parent), m_url(url.toStdString()) {}

QtUrlWrapper::QtUrlWrapper(const QString &url, const QStringList &tags, const QString &note, QObject *parent)
    : QObject(parent)
{
    std::vector<std::string> tag_vector;

    for (const auto &i : tags) {
        tag_vector.push_back(i.toStdString());
    }

    m_url = Url(url.toStdString(), tag_vector, note.toStdString());
}

Url QtUrlWrapper::coreUrl() const {
    return m_url;
}

void QtUrlWrapper::setCoreUrl(const Url& url) {
    m_url = url;
    std::vector<std::string> tags = m_url.getTags();
    QStringList tag_list;

    for (const auto &tag : tags) {
        tag_list << QString::fromStdString(tag);
    }

    emit urlChanged(QString::fromStdString(m_url.getUrl()));
    emit tagsChanged(tag_list);
    emit noteChanged(QString::fromStdString(m_url.getNote()));
}

QString QtUrlWrapper::url() const {
    return QString::fromStdString(m_url.getUrl());
}

QStringList QtUrlWrapper::tags() const {
    const auto& tags = m_url.getTags();
    QStringList qtTags;
    for (const auto& tag : tags) {
        qtTags << QString::fromStdString(tag);
    }

    return qtTags;
}

QString QtUrlWrapper::note() const {
    return QString::fromStdString(m_url.getNote());
}

void QtUrlWrapper::setUrl(const QString& url) {
    const std::string newUrl = url.toStdString();
    if (m_url.getUrl() != newUrl) {
        m_url.setUrl(newUrl);

        // Add logic to fetch new tags
        emit urlChanged(url);
    }
}

void QtUrlWrapper::setTags(const QStringList& tags) {
    std::vector<std::string> newTags;
    for (const QString& tag : tags) {
        newTags.push_back(tag.toStdString());
    }

    m_url.setTags(newTags);
    emit tagsChanged(tags);
}

void QtUrlWrapper::setNote(const QString& note) {
    const std::string newNote = note.toStdString();
    if (m_url.getNote() != newNote) {
        m_url.setNote(newNote);
        emit noteChanged(note);
    }
}

QVariantMap QtUrlWrapper::toVariantMap() const {
    return {
        {"url", url()},
        {"tags", tags()},
        {"note", note()}
    };
}

QtUrlWrapper* QtUrlWrapper::fromVariantMap(const QVariantMap& data, QObject* parent) {
    Url url(data["url"].toString().toStdString());
    url.setTags([&]{
        std::vector<std::string> tags;
        for (const QVariant& tag : data["tags"].toList()) {
            tags.push_back(tag.toString().toStdString());
        }
        return tags;
    }());
    url.setNote(data["note"].toString().toStdString());
    return new QtUrlWrapper(url, parent);
}

bool QtUrlWrapper::operator==(const QtUrlWrapper &other) const
{
    if (other.m_url.getUrl() != m_url.getUrl())
        return false;

    if (other.m_url.getNote() != m_url.getUrl())
        return false;

    if (other.m_url.getTags().size() != m_url.getTags().size())
        return false;

    std::vector<std::string> tags1 = m_url.getTags(), tags2 = other.m_url.getTags();

    for (int i = 0; i < m_url.getTags().size(); i++) {
        if (tags1.at(i) == tags2.at(i))
            return false;
    }

    return true;
}

bool QtUrlWrapper::operator!=(const QtUrlWrapper &other) const
{
    return !(*this == other);
}

QDebug operator<<(QDebug debug, const QtUrlWrapper &url)
{
    debug.nospace() << "Url: " << url.url()
                    << "\nTags: " << url.tags()
                    << "\nNotes: " << url.note();

    return debug;
}
