#include "settings.h"

QString Settings::getCommandFileLocation() const
{
    return value(COMMAND_FILE).toString();
}

void Settings::setCommandFileLocation(const QString &fullFilename)
{
    setValue(COMMAND_FILE, fullFilename);
}

Settings::Settings(const QString &filepath) :
    QSettings(filepath, QSettings::IniFormat)
{

}
