#ifndef QTTOKENWRAPPER_H
#define QTTOKENWRAPPER_H

#include <QObject>
#include <QDateTime>

#include "token.h"

class QtTokenWrapper : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tokenString READ tokenString CONSTANT)
    Q_PROPERTY(QDateTime creationDate READ creationDate CONSTANT)

    int id;

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

    int getId() const;

    void setId(int id);

    bool operator==(const QtTokenWrapper &other) const;
    bool operator!=(const QtTokenWrapper &other) const;
    // TODO: Review the next two functions for string output of objects
    operator QString() const;
    friend QDebug operator<<(QDebug debug, const QtTokenWrapper &token);
};

#endif // QTTOKENWRAPPER_H
