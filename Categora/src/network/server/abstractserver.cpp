#include "abstractserver.h"

#include <QUuid>
#include <QDebug>

AbstractServer::AbstractServer()
{}

QString AbstractServer::generateToken() {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

bool AbstractServer::isValidToken(const QString &token) {
    return m_tokens.contains(token);
}

void AbstractServer::handleRequest(const QString &request, std::function<void(const QString &)> sendResponse) {
    qDebug() << "Request received: " << request;
    QStringList parts = request.split(" ");
    if (parts.isEmpty()) {
        sendResponse("ERROR: Invalid request.\n");
        return;
    }

    QString method = parts[0].toUpper();
    QString response;

    if (method == "AUTH") {
        // Generate and store a new permanent token
        QString token = generateToken();
        m_tokens[token] = "client";
        response = "TOKEN " + token + "\n";
    } else if (method == "POST") {
        if (parts.size() < 3 || !isValidToken(parts[1])) {
            response = "ERROR: Invalid token or request format.\n";
        } else {
            QString token = parts[1];
            QString url = parts[2];
            m_url_store[token].append(url);
            response = "OK: URL stored.\n";

            // Later: dataManager.saveUrl(token, url);
        }
    } else if (method == "GET") {
        if (parts.size() < 2 || !isValidToken(parts[1])) {
            response = "ERROR: Invalid token.\n";
        } else {
            QString token = parts[1];
            if (m_url_store.contains(token)) {
                response = "URLS: " + m_url_store[token].join(", ") + "\n";
            } else {
                response = "OK: No URLs found.\n";
            }
        }
    } else {
        response = "ERROR: Unsupported method.\n";
    }

    sendResponse(response);
}
