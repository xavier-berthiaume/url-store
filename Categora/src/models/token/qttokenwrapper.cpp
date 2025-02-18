#include "qttokenwrapper.h"

QtTokenWrapper::QtTokenWrapper(QObject *parent)
    : QObject(parent),
    m_token("", std::time(nullptr)) {}  // Default empty token

QtTokenWrapper::QtTokenWrapper(const Token& token, QObject *parent)
    : QObject(parent),
    m_token(token) {}

QtTokenWrapper::QtTokenWrapper(const QString& tokenString,
                               const QDateTime& creationDate,
                               QObject *parent)
    : QObject(parent),
    m_token(tokenString.toStdString(),
            creationDate.toSecsSinceEpoch()) {}

Token QtTokenWrapper::coreToken() const {
    return m_token;
}

QString QtTokenWrapper::tokenString() const {
    return QString::fromStdString(m_token.getTokenString());
}

QDateTime QtTokenWrapper::creationDate() const {
    return QDateTime::fromSecsSinceEpoch(m_token.getCreationDate());
}

QVariantMap QtTokenWrapper::toVariantMap() const {
    return {
        {"token", tokenString()},
        {"creation_date", creationDate().toSecsSinceEpoch()}
    };
}

QtTokenWrapper* QtTokenWrapper::fromVariantMap(const QVariantMap& data,
                                               QObject* parent) {
    QDateTime date = QDateTime::fromSecsSinceEpoch(
        data["creation_date"].toLongLong()
        );
    return new QtTokenWrapper(
        data["token"].toString(),
        date,
        parent
        );
}

bool QtTokenWrapper::operator==(const QtTokenWrapper &other) const
{
    if (other.m_token.getTokenString() != m_token.getTokenString())
        return false;

    if (other.m_token.getCreationDate() != m_token.getCreationDate())
        return false;

    return true;
}

bool QtTokenWrapper::operator!=(const QtTokenWrapper &other) const
{
    return !(*this == other);
}

QDebug operator<<(QDebug debug, const QtTokenWrapper &token)
{
    debug.nospace() << "Token: " << token.tokenString()
                    << "\nCreation Date" << token.creationDate();

    return debug;
}
