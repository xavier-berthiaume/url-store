#include "../../src/models/token/qttokenwrapper.h"
#include "../../src/models/url/qturlwrapper.h"

#include <QObject>
#include <QTest>

class TestModelOperators : public QObject {
    Q_OBJECT

private slots:
    void testTokenEquals() {
        QtTokenWrapper token1("Token string", QDateTime(QDateTime::fromString("12", "yy")));
        QtTokenWrapper token2("Token string", QDateTime(QDateTime::fromString("12", "yy")));

        QVERIFY(token1 == token2);
    }

    void testTokenNotEquals() {
        QtTokenWrapper token1("Token string", QDateTime(QDateTime::currentDateTime()));
        QtTokenWrapper token2("Token string 2", QDateTime(QDateTime::currentDateTime()));

        QVERIFY(token1 != token2);

        token1.tokenString() = "Token string 2";
        token1.creationDate() = QDateTime(QDateTime::fromString("12", "yy"));

        QVERIFY(token1 != token2);
    }

    void testUrlEquals() {
        QtUrlWrapper url1("http://example.com", {"tag1", "tag2", "tag3"}, "Test note");
        QtUrlWrapper url2("http://example.com", {"tag1", "tag2", "tag3"}, "Test note");

        QVERIFY(url1 == url2);
    }

    void testUrlNotEquals() {
        QtUrlWrapper url1("http://example.com", {"tag1", "tag2", "tag3"}, "Test note");
        QtUrlWrapper url2("http://example2.com", {"tag4", "tag5", "tag6"}, "Test note two");

        QVERIFY(url1 != url2);

        url1.url() = "http://example2.com";

        QVERIFY(url1 != url2);

        url1.tags() = {"tag4", "tag5", "tag6"};

        QVERIFY(url1 != url2);
    }

};

QTEST_MAIN(TestModelOperators)
#include "testmodeloperators.moc"
