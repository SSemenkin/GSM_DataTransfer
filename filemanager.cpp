#include "filemanager.h"
#include <QFile>

FileManager::FileManager(QObject *parent)
    : QObject{parent}
{

}


QVector<QString> FileManager::getFileContent(const QString &path)
{
    QFile file(path);
    if (!file.exists()) {
        emit failed(QString("File %1 does not exists.").arg(path));
        return QVector<QString> {};
    }

    if (!file.open(QIODevice::ReadOnly)) {
        emit failed(QString("Cannot open file %1.%2").arg(path, file.errorString()));
        return QVector<QString> {};
    }

    QVector<QString> result;
    QString data = file.readAll();
    int count = data.count("\r\n");
    result.reserve(count);
    QString temporary;
    auto isStringCorrect = [](const QString &string) -> bool {
        return !string.isEmpty() && string.front() != '#';
    };
    for (int i = 0; i < data.size(); ++i) {
        if (data.at(i) == '\r') {
            i+=2;
            pushToContainerIf(result, temporary, isStringCorrect);
            temporary.clear();
        }
        temporary += data.at(i);
    }
    pushToContainerIf(result, temporary, isStringCorrect);


    return result;
}

template<typename T, typename U, typename V>
void FileManager::pushToContainerIf(T &container, const U &value, V &&validator)
{
    if (validator(value)) {
        container.push_back(value);
    }
}
