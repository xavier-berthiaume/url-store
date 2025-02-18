#include <QCoreApplication>

#include "../../src/db/sqlitedbmanager.h"
#include "../../src/models/token/qttokenwrapper.h"
#include "../../src/models/url/qturlwrapper.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    /*
    // Set application metadata
    QCoreApplication::setApplicationName("MyCLIApp");
    QCoreApplication::setApplicationVersion("1.0");

    // Set up command-line parser
    QCommandLineParser parser;
    parser.setApplicationDescription("A simple CLI application built with Qt.");
    parser.addHelpOption();
    parser.addVersionOption();

    // Add a custom option
    QCommandLineOption nameOption(QStringList() << "n" << "name", "Specify the user's name.", "name");
    parser.addOption(nameOption);

    // Process the actual command-line arguments
    parser.process(app);

    // Check if the name option was provided
    if (parser.isSet(nameOption)) {
        QString name = parser.value(nameOption);
        qDebug() << "Hello," << name << "!";
    } else {
        qDebug() << "Hello, World!";
    }
    */

    SqliteDbManager manager("");
    manager.init();

    QtTokenWrapper token("asdf-asdfasdf-asdfasdfas-asdfasdfa", QDateTime(QDateTime::currentDateTime()));
    QtTokenWrapper *readToken = nullptr;

    QtUrlWrapper url("https://example.com", {"tag1", "tag2", "tag3"}, "This is my note");
    QtUrlWrapper *readUrl = nullptr;

    qDebug() << "Saved token: " << manager.saveToken(token);
    qDebug() << "Saved url: " << manager.saveUrl(url);

    qDebug() << "Is token read successful: " << manager.readToken(1, readToken);
    qDebug() << "Is url read successful: " << manager.readUrl(1, readUrl);

    qDebug() << "This is the original token\n" << token.tokenString() << " - " << token.creationDate();
    qDebug() << "This is the read token\n" << readToken->tokenString() << " - " << readToken->creationDate();
    qDebug() << "This is the read url\n" << readUrl;

    qDebug() << "Read token matches initial token: " << (*readToken == token);
    qDebug() << "Read url matches initial url: " << (*readUrl == url);

    delete readToken;
    delete readUrl;

    return app.exec();
}
