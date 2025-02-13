#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

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

    return app.exec();
}
