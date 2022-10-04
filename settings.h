#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QStandardPaths>
#include <QCoreApplication>
#include "singleton.h"

#define COMMAND_FILE "command_file_location"


class Settings : protected QSettings, public Singleton<Settings>
{
    Q_OBJECT

public:
    [[nodiscard]] QString getCommandFileLocation() const;
    void setCommandFileLocation(const QString &fullFilename);
protected:
    Settings(const QString &filepath =
            QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/" +
            qApp->applicationName() + "/" + "config.ini");
private:
};

#endif // SETTINGS_H
