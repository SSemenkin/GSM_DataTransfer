#ifndef CONTROLLERHOLDER_H
#define CONTROLLERHOLDER_H

#include <QObject>
#include <QMap>
#include <QSharedPointer>
#include "singleton.h"

class Telnet;

class ControllerHolder : public QObject, public Singleton<ControllerHolder>
{
    Q_OBJECT
public:
    void collectData();
signals:
    void dataCollected(const QMap<QString, QMap<QString, QMap<QString, QStringList>>> &data);
protected:
    explicit ControllerHolder();
private:
    static bool init();
    void parseOutput(const QString &answer);
private:
    QVector<QSharedPointer<Telnet>> m_controllers;
    // [controller][tranceiverGroup][cell/param]
    QMap<QString, QMap<QString, QMap<QString, QStringList>>> m_data;
    static QVector<QString> m_commands;
};

#endif // CONTROLLERHOLDER_H
