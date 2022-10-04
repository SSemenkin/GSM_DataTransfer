#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QStandardPaths>
#include <QCoreApplication>
#include "Singleton.h"

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
#define COMMAND_FILE "command_file_location"
};

#endif // SETTINGS_H
