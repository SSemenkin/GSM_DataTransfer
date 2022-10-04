#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include "singleton.h"

class FileManager : public QObject, public Singleton<FileManager>
{
    Q_OBJECT
public:
    [[nodiscard]] QVector<QString> getFileContent(const QString &path);

signals:
    void failed(const QString &description);
protected:
    explicit FileManager(QObject *parent = nullptr);
    template<typename T, typename U, typename V>
    void pushToContainerIf(T &container, const U &value, V&& validator);
};

#endif // FILEMANAGER_H
