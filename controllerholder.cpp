#include "controllerholder.h"
#include "filemanager.h"
#include "settings.h"

ControllerHolder::ControllerHolder()
    : QObject{}
{
    if (!ControllerHolder::init()) {
        return;
    }
}

bool ControllerHolder::init()
{
    if (!ControllerHolder::m_commands.isEmpty()) {
        return true;
    }

    FileManager *fileManager = FileManager::instance();
    m_commands = fileManager->getFileContent(Settings::instance()->getCommandFileLocation());

    return !m_commands.isEmpty();
}

void ControllerHolder::collectData()
{

}

void ControllerHolder::parseOutput(const QString &answer)
{

}
