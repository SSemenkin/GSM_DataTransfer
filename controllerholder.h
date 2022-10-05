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
    enum class State
    {
        Idle,
        Busy
    };

    void collectData();
    State state() const;
signals:
    void dataCollected(const QMap<QString, QMap<QString, QMap<QString, QString>>> &data);
    void errorReceived(const QString &error);
protected:
    explicit ControllerHolder();
private:
    bool init();
    bool initControllers();
    void parseOutput(QString answer);
    void receiveAnswer(const QString &responce);
private:
    QVector<QSharedPointer<Telnet>> m_controllers;
    // [controller][cell][param] - > value
    QMap<QString, QMap<QString, QMap<QString, QString>>> m_data;
    static QVector<QString> s_commans;
    size_t m_receive {0};
    State m_state {State::Idle};
};

#endif // CONTROLLERHOLDER_H
