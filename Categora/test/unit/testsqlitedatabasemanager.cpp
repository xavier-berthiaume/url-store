#include "../../src/db/sqlitedbmanager.h"
#include "testsqlitedatabasemanager.h"

#include <QDateTime>
#include <QtTest/QtTest>

TestDatabaseManager::TestDatabaseManager(QObject *parent)
    : QObject(parent) {}

void TestDatabaseManager::initTestCase()
{
    dbManager = new SqliteDbManager(testDbPath, this);
    QVERIFY(dbManager->init());
}

void TestDatabaseManager::cleanupTestCase()
{
    dbManager->close();
    delete dbManager;
}

void TestDatabaseManager::init() {}
void TestDatabaseManager::cleanup()
{
    // Clear all data between tests
    dbManager->deleteUrl(testId);
    dbManager->deleteToken(testId);
}

// Helper function implementations
bool TestDatabaseManager::tokenExistsInDatabase(quint32 id)
{
    QtTokenWrapper *token;
    return dbManager->readToken(id, token) && token != nullptr;
}

bool TestDatabaseManager::urlExistsInDatabase(quint32 id)
{
    QtUrlWrapper *url;
    return dbManager->readUrl(id, url) && url != nullptr;
}

// Token tests
void TestDatabaseManager::testSaveToken()
{
    const std::string testToken = "test_token_123";
    const std::time_t testDate = QDateTime::currentSecsSinceEpoch();

    Token token(testToken, testDate);
    QtTokenWrapper qtToken(token);

    // Test save
    QVERIFY(dbManager->saveToken(qtToken));
    QVERIFY(tokenExistsInDatabase(testId));
}

void TestDatabaseManager::testReadToken()
{
    // First save a token
    const std::string originalToken = "test_read_token";
    const std::time_t originalDate = QDateTime::currentSecsSinceEpoch();
    Token token(originalToken, originalDate);
    dbManager->saveToken(QtTokenWrapper(token));

    // Test read
    QtTokenWrapper *readToken;
    QVERIFY(dbManager->readToken(testId, readToken));
    QVERIFY(readToken != nullptr);
    QCOMPARE(readToken->tokenString().toStdString(), originalToken);
    QCOMPARE(readToken->creationDate().toSecsSinceEpoch(), originalDate);
}

void TestDatabaseManager::testUpdateToken()
{
    // Create initial token
    dbManager->saveToken(QtTokenWrapper(Token("old_token", QDateTime::currentSecsSinceEpoch())));

    // Update token
    const std::string updatedToken = "updated_token";
    const std::time_t updatedDate = QDateTime::currentSecsSinceEpoch() + 100;
    Token newToken(updatedToken, updatedDate);

    QVERIFY(dbManager->updateToken(testId, QtTokenWrapper(newToken)));

    // Verify update
    QtTokenWrapper *readToken;
    QVERIFY(dbManager->readToken(testId, readToken));
    QCOMPARE(readToken->tokenString().toStdString(), updatedToken);
    QCOMPARE(readToken->creationDate().toSecsSinceEpoch(), updatedDate);
}

void TestDatabaseManager::testDeleteToken()
{
    // Create token first
    dbManager->saveToken(QtTokenWrapper(Token("to_delete", QDateTime::currentSecsSinceEpoch())));

    // Test delete
    QVERIFY(dbManager->deleteToken(testId));
    QVERIFY(!tokenExistsInDatabase(testId));
}

// URL tests
void TestDatabaseManager::testSaveUrl()
{
    Url url("https://example.com");
    url.setNote("Test note");
    url.setTags({"tag1", "tag2"});

    QtUrlWrapper qtUrl(url);

    QVERIFY(dbManager->saveUrl(qtUrl));
    QVERIFY(urlExistsInDatabase(testId));
}

void TestDatabaseManager::testReadUrl()
{
    // Create test URL
    QString originalUrl = "https://test.com";
    QString originalNote = "Test note";
    QStringList originalTags = {"test", "qtest"};

    dbManager->saveUrl(QtUrlWrapper(originalUrl, originalTags, originalNote));

    // Test read
    QtUrlWrapper *readUrl;
    QVERIFY(dbManager->readUrl(testId, readUrl));
    QVERIFY(readUrl != nullptr);
    QCOMPARE(readUrl->url(), originalUrl);
    QCOMPARE(readUrl->note(), originalNote);
    QCOMPARE(readUrl->tags(), originalTags);
}

void TestDatabaseManager::testUpdateUrl()
{
    // Create initial URL
    Url original("https://original.com");
    original.setNote("Original note");
    original.setTags({"oldtag"});
    dbManager->saveUrl(QtUrlWrapper(original));

    // Updated values
    QtUrlWrapper qtUpdated("https://updated.com", {"newtag1", "newtag2"}, "Updated note");

    QVERIFY(dbManager->updateUrl(testId, qtUpdated));

    // Verify update
    QtUrlWrapper *readUrl;
    QVERIFY(dbManager->readUrl(testId, readUrl));
    QCOMPARE(readUrl->url(), qtUpdated.url());
    QCOMPARE(readUrl->note(), qtUpdated.note());
    QCOMPARE(readUrl->tags(), qtUpdated.tags());
}

void TestDatabaseManager::testDeleteUrl()
{
    // Create URL first
    dbManager->saveUrl(QtUrlWrapper("https://todelete.com"));

    QVERIFY(dbManager->deleteUrl(testId));
    QVERIFY(!urlExistsInDatabase(testId));
}

void TestDatabaseManager::testUrlTags()
{
    // Test tag persistence
    QStringList tags = {"tag1", "tag2", "tag3"};
    QtUrlWrapper qtUrl("https://example.com", tags, "");

    QVERIFY(dbManager->saveUrl(qtUrl));

    QtUrlWrapper *readUrl;
    QVERIFY(dbManager->readUrl(testId, readUrl));
    QCOMPARE(readUrl->tags().size(), tags.size());
    QVERIFY(readUrl->tags() == tags);
}

void TestDatabaseManager::testTransactionRollback()
{
    // Test failed transaction doesn't save partial data
    Url invalidUrl(""); // Invalid empty URL
    invalidUrl.setTags({"should", "not", "save"});

    QVERIFY(!dbManager->saveUrl(QtUrlWrapper(invalidUrl))); // Should fail

    // Verify nothing was saved
    QVERIFY(!urlExistsInDatabase(testId));
}

QTEST_MAIN(TestDatabaseManager)
#include "testsqlitedatabasemanager.moc"
