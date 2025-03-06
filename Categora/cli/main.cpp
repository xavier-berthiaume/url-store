#include <QCoreApplication>

#include "../../src/db/sqlitedbmanager.h"
#include "../../src/network/api/deepseek/deepseekapimanager.h"
#include "../../src/network/server/tcpserver.h"
#include "envloader.h"

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

    const QMap<QString, QString> env = EnvLoader::loadEnv("../.env");
    if (!env.contains("DEEPSEEK_API_KEY")) {
        qCritical() << "Missing DEEPSEEK_API_KEY in .env file";
        return 1;
    }

    SqliteDbManager db_manager("/home/xavier/Documents/database.sqlite");
    db_manager.init();

    DeepSeekApiManager api_manager;
    api_manager.setApiKey(env["DEEPSEEK_API_KEY"]);

    TcpServer server(&db_manager, &api_manager);
    server.startServer(12345);

    return app.exec();
}
