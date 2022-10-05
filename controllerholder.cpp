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

    for (auto &controller: qAsConst(m_controllers)) {
        connect(controller.get(), &Telnet::commandExecuted, this, &ControllerHolder::receiveAnswer);
        connect(controller.get(), &Telnet::errorOccured, this, &ControllerHolder::errorReceived);
    }
}

bool ControllerHolder::init()
{
    if (!ControllerHolder::s_commans.isEmpty()) {
        return true;
    }

    FileManager *fileManager = FileManager::instance();
    s_commans = fileManager->getFileContent(Settings::instance()->getCommandFileLocation());

    return !s_commans.isEmpty() && initControllers();
}

bool ControllerHolder::initControllers()
{
    QEventLoop loop;
    std::vector<bool> results;
    QList<ControllerInfo> d = Settings::instance()->getControllersInfos();
    for (int i = 0; i < d.size(); ++i) {
        m_controllers.push_back(QSharedPointer<Telnet>(new Telnet("", d.at(i).hostname,
                                                                      d.at(i).username,
                                                                      d.at(i).password

                                                                  )));
        connect(m_controllers.last().data(), &Telnet::loginStateChanged, this, [&loop, &results] (bool state) {
            results.push_back(state);
            loop.quit();
        });
        m_controllers.last()->connectToNode();
        loop.exec();
    }
    return std::find(results.begin(), results.end(), false) != results.end();
}

void ControllerHolder::collectData()
{
    if (state() == State::Busy) {
        return;
    }
    m_data.clear();
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

void ControllerHolder::parseOutput(QString answer)
{
    const QString controller = static_cast<Telnet*>(sender())->parsedTitle();

    QStringList rows = answer.remove('\r').split('\n');

    QString cellname;

    for(int rowIndex = 0; rowIndex < rows.size() - 1; ++rowIndex) {
        const QString &row = rows.at(rowIndex);

        if (row == "END" || row.isEmpty() || (!row.isEmpty() && rows.at(rowIndex + 1).isEmpty())) {
            continue;
        }

        if (row == "CELL") {
            cellname = rows.at(rowIndex + 1);
            rowIndex += 3;
            continue;
        }

        QStringList params = row.split(' ', Qt::SkipEmptyParts);
        QStringList paramsValues;

        const QString &paramString = rows.at(rowIndex + 1);

        for (int paramIndex = 0; paramIndex < params.size(); ++paramIndex) {
            int beginParam = row.indexOf(params.at(paramIndex));
            int endIndex = paramIndex < params.size() - 1 ? row.indexOf(params.at(paramIndex + 1)) - beginParam :
                                                            beginParam + params.at(paramIndex).length();
            QString paramValue = paramString.mid(beginParam, endIndex);
            m_data[controller][cellname][params.at(paramIndex)] = paramValue;
        }

    }
}

void ControllerHolder::receiveAnswer(const QString &responce)
{
    ++m_receive;

    parseOutput(responce);

    if (m_receive == size_t(s_commans.size() * m_controllers.size())) {
        m_receive = 0;
        m_state = State::Idle;
    }
}
