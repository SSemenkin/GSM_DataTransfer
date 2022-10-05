#include "settings.h"

QString Settings::getCommandFileLocation() const
{
    return value(COMMAND_FILE).toString();
}

void Settings::setCommandFileLocation(const QString &fullFilename)
{
    setValue(COMMAND_FILE, fullFilename);
}

void Settings::setControllersInfos(const QList<ControllerInfo> &controllersList)
{
    if (controllersList.isEmpty()) {
        return;
    }

    beginGroup(decodeEncodeData("Controllers"));
    remove("");

    setValue(decodeEncodeData("controllers_count"), controllersList.size());

    for (int i = 0; i < controllersList.size(); ++i) {
        setValue(decodeEncodeData(QString("hostname_%1").arg(i)), decodeEncodeData(controllersList.at(i).hostname));
        setValue(decodeEncodeData(QString("username_%1").arg(i)), decodeEncodeData(controllersList.at(i).username));
        setValue(decodeEncodeData(QString("password_%1").arg(i)), decodeEncodeData(controllersList.at(i).password));
    }
    endGroup();
}

// static
QString Settings::decodeEncodeData(const QString &input, const QString &key)
{
    QByteArray inputData = input.toLatin1();
    const char *rawInput = inputData.data();
    int inputLength = inputData.length();

    QByteArray keyData = key.toLatin1();
    const char *rawKey = keyData.data();
    int keyLength = keyData.length();

    QString result;

    char r[inputLength];

    for (int i = 0; i < inputLength; ++i) {
        r[i] = rawInput[i] ^ rawKey[i % keyLength];
    }

    result = QString::fromLatin1(r, inputLength);
    return result;
}

QList<ControllerInfo> Settings::getControllersInfos()
{
    QList<ControllerInfo> result;

    beginGroup(decodeEncodeData("Controllers"));
    int count = value(decodeEncodeData("controllers_count"), 0).toInt();

    ControllerInfo temporary;
    for (int i = 0; i < count; ++i) {
       temporary.hostname = decodeEncodeData(value(decodeEncodeData(QString("hostname_%1").arg(i))).toString());
       temporary.username = decodeEncodeData(value(decodeEncodeData(QString("username_%1").arg(i))).toString());
       temporary.password = decodeEncodeData(value(decodeEncodeData(QString("password_%1").arg(i))).toString());
       result.push_back(temporary);
    }
    endGroup();

    return result;
}

Settings::Settings(const QString &filepath) :
    QSettings(filepath, QSettings::IniFormat)
{

}
