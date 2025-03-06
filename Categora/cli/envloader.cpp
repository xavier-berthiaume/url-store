#include "envloader.h"

QMap<QString, QString> EnvLoader::loadEnv(const QString& path) {
    qDebug() << "Parsing env file at " << path;
    QMap<QString, QString> env;
    QString envPath = path.isEmpty() ?
                          QCoreApplication::applicationDirPath() + "/.env" : path;

    QFile file(envPath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open .env file:" << envPath;
        return env;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.startsWith("#") || line.isEmpty()) continue;

        QStringList parts = line.split("=");
        if (parts.size() >= 2) {
            QString key = parts[0].trimmed();
            QString value = parts[1].trimmed();
            // Remove quotes if present
            if (value.startsWith('"') && value.endsWith('"')) {
                value = value.mid(1, value.length() - 2);
            }
            env[key] = value;

            qDebug() << QString("Found [%1] = %2").arg(key, value);
        }
    }
    file.close();
    return env;
}
