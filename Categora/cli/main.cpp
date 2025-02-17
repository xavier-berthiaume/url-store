#include <QCoreApplication>

#include "../src/db/sqlitedbmanager.h"
#include "../src/models/token/token.h"
#include "../src/models/url/url.h"

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

    Token token = Token("This is a token ;)", 123123123);

    SqliteDbManager manager = SqliteDbManager("");
    manager.init();
    manager.saveToken(token);

    std::shared_ptr<Token> token2;
    manager.readToken(1, token2);
    qDebug() << QString::fromStdString(token2->getTokenString());
    manager.close();

    return app.exec();
}
