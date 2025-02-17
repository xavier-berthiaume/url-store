#ifndef TESTDATABASEMANAGER_H
#define TESTDATABASEMANAGER_H

#include <QObject>
#include <QtTest/QtTest>
#include "sqlitedbmanager.h"

class TestDatabaseManager : public QObject
{
    Q_OBJECT

public:
    explicit TestDatabaseManager(QObject *parent = nullptr);

private slots:
    void initTestCase();    // Runs once at start
    void cleanupTestCase(); // Runs once at end
    void init();            // Runs before each test
    void cleanup();         // Runs after each test

    // Token tests
    void testSaveToken();
    void testReadToken();
    void testUpdateToken();
    void testDeleteToken();

    // URL tests
    void testSaveUrl();
    void testReadUrl();
    void testUpdateUrl();
    void testDeleteUrl();
    void testUrlTags();
    void testTransactionRollback();

private:
    SqliteDbManager* dbManager;
    const QString testDbPath = ":memory:"; // Use in-memory database for tests
    const quint32 testId = 1; // Test with ID=1 for all operations

    // Helper functions
    bool tokenExistsInDatabase(quint32 id);
    bool urlExistsInDatabase(quint32 id);
};

#endif // TESTDATABASEMANAGER_H
