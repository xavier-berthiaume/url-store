#ifndef QTTOKENWRAPPER_H
#define QTTOKENWRAPPER_H

#include <QObject>
#include <QDateTime>

#include "token.h"

class QtTokenWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString token READ token CONSTANT)
    Q_PROPERTY(QDateTime creationDate READ creationDate CONSTANT)

    Token m_token;

public:
    explicit QtTokenWrapper(QObject *parent = nullptr);
    explicit QtTokenWrapper(const Token& token, QObject *parent = nullptr);
    explicit QtTokenWrapper(const QString& tokenString,
                            const QDateTime& creationDate,
                            QObject *parent = nullptr);

    Token coreToken() const;
    QVariantMap toVariantMap() const;
    static QtTokenWrapper* fromVariantMap(const QVariantMap& data,
                                          QObject* parent = nullptr);

    QString tokenString() const;
    QDateTime creationDate() const;

};

#endif // QTTOKENWRAPPER_H
