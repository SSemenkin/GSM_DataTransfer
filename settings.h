#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>
#include <QStandardPaths>
#include <QCoreApplication>
#include "singleton.h"

#define COMMAND_FILE "command_file_location"

struct ControllerInfo
{
    QString hostname;
    QString username;
    QString password;
};


class Settings : protected QSettings, public Singleton<Settings>
{
    Q_OBJECT

public:
    [[nodiscard]] QString getCommandFileLocation() const;
    void setCommandFileLocation(const QString &fullFilename);

    [[nodiscard]] QList<ControllerInfo> getControllersInfos();
    void setControllersInfos(const QList<ControllerInfo> &controllersList);

    static QString decodeEncodeData(const QString &input, const QString &key = "%31_)*&z;");

protected:
    Settings(const QString &filepath =
            QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation) + "/" +
            qApp->applicationName() + "/" + "config.ini");
private:
};

#endif // SETTINGS_H
