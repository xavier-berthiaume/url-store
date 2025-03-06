#include "abstractserver.h"

#include "models/token/qttokenwrapper.h"

#include <QUuid>
#include <QDebug>

AbstractServer::AbstractServer(AbstractDbManager *m_db, ApiManager *m_api, QObject *parent)
    : m_db(m_db)
    , m_api(m_api)
    , QObject(parent)
{
    connect(m_api, &ApiManager::tagsFetched, this, &AbstractServer::handleTagsFetched);
}

QString AbstractServer::generateToken() {
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}

bool AbstractServer::isValidToken(const QString &token) {
    QtTokenWrapper *readToken = nullptr;
    if (!m_db->readToken(token, readToken))
        return false;

    if (readToken == nullptr)
        return false;

    return true;
}

QString AbstractServer::handleAuth()
{
    QtTokenWrapper token(generateToken(), QDateTime(QDateTime::currentDateTime()));
    if (!m_db->saveToken(token)) {
        return "ERROR: Internal server error.\n";
    }

    return "TOKEN " + token.tokenString() + "\n";
}

QString AbstractServer::handlePost(const QString &tokenString, const QString &urlString)
{
    QtTokenWrapper *readToken = nullptr;
    QtUrlWrapper url;

    // Token validation
    if (!m_db->readToken(tokenString, readToken)) {
        return "ERROR: Internal server error.\n";
    }

    if (readToken == nullptr) {
        return "ERROR: Invalid token.\n";
    }

    // Check to see if the url hasn't already been stored

    // Getting tags for the url
    url.setUrl(urlString);
    m_api->fetchTags(urlString);

    // Third, store the url, matching it to the correct token
    m_pending_storage[urlString] = tokenString;

    return "OK: URL stored.\n";
}

QString AbstractServer::handleGet(const QString &tokenString)
{
    /*
    if (m_url_store.contains(token)) {
        return "URLS: " + m_url_store[token].join(", ") + "\n";
    }
    */

    return "OK: No URLs found.\n";
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
        response = handleAuth();
    } else if (method == "POST") {
        if (parts.size() < 3 || !isValidToken(parts[1])) {
            response = "ERROR: Invalid token or request format.\n";
        } else {
            response = handlePost(parts[1], parts[2]);
        }
    } else if (method == "GET") {
        if (parts.size() < 2 || !isValidToken(parts[1])) {
            response = "ERROR: Invalid token.\n";
        } else {
            response = handleGet(parts[1]);
        }
    } else {
        response = "ERROR: Unsupported method.\n";
    }

    sendResponse(response);
}

void AbstractServer::handleTagsFetched(const QString &urlString, const QStringList &tags)
{
    // First find the correct token
    QtTokenWrapper *token;

    if (!m_db->readToken(m_pending_storage[urlString], token)) {
        // Bad token
        m_pending_storage.remove(urlString);
        return;
    }

    // Create the complete Url wrapper
    QtUrlWrapper url = QtUrlWrapper(urlString, tags, "", this);

    // Save the url
    m_db->saveUrl(url);

    // Tie the url to the token
    if(!m_db->addUrlOwner(*token, url)) {
        return;
    }

    // Remove the url and token from the pending pool
    m_pending_storage.remove(urlString);
}
