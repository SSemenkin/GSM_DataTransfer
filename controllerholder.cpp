#include "controllerholder.h"
#include "filemanager.h"
#include "settings.h"
#include "network/telnet.h"

QVector<QString> ControllerHolder::s_commans {};

ControllerHolder::ControllerHolder()
    : QObject{}
{
    if (!ControllerHolder::init()) {
        return;
    }
}

bool ControllerHolder::init()
{
    if (!ControllerHolder::s_commans.isEmpty()) {
        return true;
    }

    FileManager *fileManager = FileManager::instance();
    s_commans = fileManager->getFileContent(Settings::instance()->getCommandFileLocation());

    return !s_commans.isEmpty();
}

void ControllerHolder::collectData()
{
    if (state() == State::Busy) {
        return;
    }

    m_state = State::Busy;

    for (const QString &command : qAsConst(ControllerHolder::s_commans)) {
        for (QSharedPointer<Telnet> &controller : m_controllers) {
            controller->executeCommand(command);
        }
    }
}

ControllerHolder::State ControllerHolder::state() const
{
    return m_state;
}

void ControllerHolder::parseOutput(const QString &answer)
{
    ++m_receive;

    if (m_receive == size_t(s_commans.size() * m_controllers.size())) {
        m_receive = 0;
        m_state = State::Idle;
    }
}
