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
    std::unique_ptr<Token> token;
    return dbManager->readToken(id, token) && token != nullptr;
}

bool TestDatabaseManager::urlExistsInDatabase(quint32 id)
{
    std::unique_ptr<Url> url;
    return dbManager->readUrl(id, url) && url != nullptr;
}

// Token tests
void TestDatabaseManager::testSaveToken()
{
    const std::string testToken = "test_token_123";
    const std::time_t testDate = QDateTime::currentSecsSinceEpoch();

    Token token(testToken, testDate);

    // Test save
    QVERIFY(dbManager->saveToken(token));
    QVERIFY(tokenExistsInDatabase(testId));
}

void TestDatabaseManager::testReadToken()
{
    // First save a token
    const std::string originalToken = "test_read_token";
    const std::time_t originalDate = QDateTime::currentSecsSinceEpoch();
    dbManager->saveToken(Token(originalToken, originalDate));

    // Test read
    std::unique_ptr<Token> readToken;
    QVERIFY(dbManager->readToken(testId, readToken));
    QVERIFY(readToken != nullptr);
    QCOMPARE(readToken->getTokenString(), originalToken);
    QCOMPARE(readToken->getCreationDate(), originalDate);
}

void TestDatabaseManager::testUpdateToken()
{
    // Create initial token
    dbManager->saveToken(Token("old_token", QDateTime::currentSecsSinceEpoch()));

    // Update token
    const std::string updatedToken = "updated_token";
    const std::time_t updatedDate = QDateTime::currentSecsSinceEpoch() + 100;
    Token newToken(updatedToken, updatedDate);

    QVERIFY(dbManager->updateToken(testId, newToken));

    // Verify update
    std::unique_ptr<Token> readToken;
    QVERIFY(dbManager->readToken(testId, readToken));
    QCOMPARE(readToken->getTokenString(), updatedToken);
    QCOMPARE(readToken->getCreationDate(), updatedDate);
}

void TestDatabaseManager::testDeleteToken()
{
    // Create token first
    dbManager->saveToken(Token("to_delete", QDateTime::currentSecsSinceEpoch()));

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

    QVERIFY(dbManager->saveUrl(url));
    QVERIFY(urlExistsInDatabase(testId));
}

void TestDatabaseManager::testReadUrl()
{
    // Create test URL
    const std::string originalUrl = "https://test.com";
    const std::string originalNote = "Test note";
    const std::vector<std::string> originalTags = {"test", "qtest"};

    Url url(originalUrl);
    url.setNote(originalNote);
    url.setTags(originalTags);
    dbManager->saveUrl(url);

    // Test read
    std::unique_ptr<Url> readUrl;
    QVERIFY(dbManager->readUrl(testId, readUrl));
    QVERIFY(readUrl != nullptr);
    QCOMPARE(readUrl->getUrl(), originalUrl);
    QCOMPARE(readUrl->getNote(), originalNote);
    QCOMPARE(readUrl->getTags(), originalTags);
}

void TestDatabaseManager::testUpdateUrl()
{
    // Create initial URL
    Url original("https://original.com");
    original.setNote("Original note");
    original.setTags({"oldtag"});
    dbManager->saveUrl(original);

    // Update values
    Url updated("https://updated.com");
    updated.setNote("Updated note");
    updated.setTags({"newtag1", "newtag2"});

    QVERIFY(dbManager->updateUrl(testId, updated));

    // Verify update
    std::unique_ptr<Url> readUrl;
    QVERIFY(dbManager->readUrl(testId, readUrl));
    QCOMPARE(readUrl->getUrl(), updated.getUrl());
    QCOMPARE(readUrl->getNote(), updated.getNote());
    QCOMPARE(readUrl->getTags(), updated.getTags());
}

void TestDatabaseManager::testDeleteUrl()
{
    // Create URL first
    dbManager->saveUrl(Url("https://todelete.com"));

    QVERIFY(dbManager->deleteUrl(testId));
    QVERIFY(!urlExistsInDatabase(testId));
}

void TestDatabaseManager::testUrlTags()
{
    // Test tag persistence
    const std::vector<std::string> tags = {"tag1", "tag2", "tag3"};
    Url url("https://tags.com");
    url.setTags(tags);

    QVERIFY(dbManager->saveUrl(url));

    std::unique_ptr<Url> readUrl;
    QVERIFY(dbManager->readUrl(testId, readUrl));
    QCOMPARE(readUrl->getTags().size(), tags.size());
    QVERIFY(readUrl->getTags() == tags);
}

void TestDatabaseManager::testTransactionRollback()
{
    // Test failed transaction doesn't save partial data
    Url invalidUrl(""); // Invalid empty URL
    invalidUrl.setTags({"should", "not", "save"});

    QVERIFY(!dbManager->saveUrl(invalidUrl)); // Should fail

    // Verify nothing was saved
    QVERIFY(!urlExistsInDatabase(testId));
}

QTEST_MAIN(TestDatabaseManager)
#include "TestDatabaseManager.moc"
