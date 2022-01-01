#include "ofonopluginconnector.h"

void KU_Ofono_PluginConnector::pluginSlot(const QString& signal, const QVariantMap& data)
{
    if (signal == "dial")
    {
        if (!data["number"].isNull() && !data["number"].toString().isEmpty())
            emit callSignal(data["number"].toString());
    }
}

void KU_Ofono_PluginConnector::connectModem()
{
    connect(this->ofonoModem, &QOfonoModem::reportError, this, [=](QString const& errorString) {
        emit log("reportError: " + errorString);
    });
    connect(this->ofonoModem, &QOfonoModem::poweredChanged, this, [=](bool powered) {
        emit log(QString("poweredChanged: %1").arg(powered));
        this->ofonoModem->setOnline(true);
    });
    connect(this->ofonoModem, &QOfonoModem::onlineChanged, this, [=](bool online) {
        emit log(QString("onlineChanged: %1").arg(online));
        this->ofonoVoiceCallManager = new QOfonoVoiceCallManager(this);
        this->ofonoVoiceCallManager->setModemPath(this->ofonoModem->modemPath());
        connectVoiceCallManager();
    });
    emit log(QString("setPowered"));
    // this->ofonoModem->setPowered(true);
    QDBusMessage    message = QDBusMessage::createMethodCall("org.ofono",
                                                          this->ofonoModem->modemPath(),
                                                          "org.ofono.Modem",
                                                          "SetProperty");
    QList<QVariant> arguments;
    arguments << QString("Powered");
    arguments << QVariant::fromValue(QDBusVariant(true));

    message.setArguments(arguments);

    bool res = QDBusConnection::systemBus().send(message);
    emit log(QString("QDBusConnection::systemBus().send(message): %1").arg(res));
}

void KU_Ofono_PluginConnector::connectVoiceCallManager()
{
    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::callAdded, this, [=](const QString& call) {
        emit log("callAdded: " + call);

        if (calls.contains(call))
            return;

        auto ofonoVoiceCall = new QOfonoVoiceCall(this);
        ofonoVoiceCall->setVoiceCallPath(call);
        connect(ofonoVoiceCall, &QOfonoVoiceCall::lineIdentificationChanged, this, &KU_Ofono_PluginConnector::callsChanged);
        connect(ofonoVoiceCall, &QOfonoVoiceCall::stateChanged, this, &KU_Ofono_PluginConnector::callsChanged);
        connect(ofonoVoiceCall, &QOfonoVoiceCall::lineIdentificationChanged, this, [=]() {
            emit log("lineIdentificationChanged: " + call + " " + ofonoVoiceCall->lineIdentification());
        });
        connect(ofonoVoiceCall, &QOfonoVoiceCall::stateChanged, this, [=]() {
            emit log("stateChanged: " + call + " " + ofonoVoiceCall->state());
        });
        calls.insert(call, ofonoVoiceCall);

        emit callsChanged();

        QString address = call.split('/').at(5);
        address.replace("dev_", "");
        address.replace("_", ":");
        QProcess* recordProcess = new QProcess(this);
        connect(recordProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [this](int exitCode, QProcess::ExitStatus exitStatus) {
            emit log("Record process finished: " + QString::number(exitCode) + " " + QString::number(exitStatus == QProcess::ExitStatus::NormalExit));
        });
        connect(recordProcess, &QProcess::readyReadStandardOutput, this, [this, recordProcess]() {
            emit log(QString("Record process output: ") + recordProcess->readAllStandardOutput());
        });
        connect(recordProcess, &QProcess::readyReadStandardError, this, [this, recordProcess]() {
            emit log(QString("Record process error: ") + recordProcess->readAllStandardError());
        });
        recordProcess->start("/bin/sh -c \"LIBASOUND_THREAD_SAFE=0 /usr/bin/arecord -f s16_le | /usr/bin/aplay -D bluealsa:DEV=" + address + ",PROFILE=sco\"");
        emit log("Record process started for: " + address);
        if (!recordProcess->waitForStarted())
            emit log("Record process failed: " + recordProcess->errorString() + " " + QString::number(recordProcess->exitCode()) + " " + recordProcess->readAllStandardError());
    });

    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::callRemoved, this, [=](const QString& call) {
        emit log("callRemoved: " + call);
        if (calls.contains(call))
        {
            calls[call]->deleteLater();
            calls.remove(call);
        }
        emit callsChanged();
    });

    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::dialComplete, this, [=](bool status) {
        emit log(QString("dialComplete: [%1] ").arg(status) + this->ofonoVoiceCallManager->errorMessage());
    });
}

KU_Ofono_PluginConnector::KU_Ofono_PluginConnector(QObject* parent)
    : KU::PLUGIN::PluginConnector(parent)
{
}

void KU_Ofono_PluginConnector::setup()
{
    emit log(QString("setup Ofono"));

    this->ofonoManager = new QOfonoManager(this);

    connect(this->ofonoManager, &QOfonoManager::availableChanged, this, [=](bool available) {
        if (this->ofonoManager->modems().size() == 0)
        {
            emit log(QString("no modem"));
            return;
        }

        emit log(QString("availableChanged [%1] ").arg(available) + this->ofonoManager->modems().at(0));
        this->ofonoModem = new QOfonoModem(this);
        this->ofonoModem->setModemPath(this->ofonoManager->modems().at(0));
        connectModem();
    });
    connect(this->ofonoManager, &QOfonoManager::modemAdded, this, [=](const QString& modem) {
        emit log("modemAdded: " + modem);
    });
    connect(this->ofonoManager, &QOfonoManager::modemRemoved, this, [=](const QString& modem) {
        emit log("modemRemoved: " + modem);
    });
    connect(this->ofonoManager, &QOfonoManager::defaultModemChanged, this, [=](const QString& modem) {
        emit log("defaultModemChanged: " + modem);
    });
}

void KU_Ofono_PluginConnector::call(QString const& number)
{
    emit log(QString("call Ofono"));
    if (this->ofonoVoiceCallManager != nullptr)
        this->ofonoVoiceCallManager->dial(number, "enabled");
}

void KU_Ofono_PluginConnector::hangupAll()
{
    emit log(QString("hangupAll Ofono"));
    if (this->ofonoVoiceCallManager != nullptr)
        this->ofonoVoiceCallManager->hangupAll();
    this->calls.clear();
    emit callsChanged();
}

QStringList KU_Ofono_PluginConnector::callsAliases() const
{
    return this->calls.keys();
}

void KU_Ofono_PluginConnector::answer(const QString& call)
{
    if (this->calls.contains(call))
        this->calls[call]->answer();
}

void KU_Ofono_PluginConnector::hangup(const QString& call)
{
    if (this->calls.contains(call))
        this->calls[call]->hangup();
}

QString KU_Ofono_PluginConnector::state(const QString& call) const
{
    if (this->calls.contains(call))
        return this->calls[call]->state();
    return QString();
}

QString KU_Ofono_PluginConnector::lineIdentification(const QString& call) const
{
    if (this->calls.contains(call))
        return this->calls[call]->lineIdentification();
    return QString();
}

QString KU_Ofono_PluginConnector::incomingLine(const QString& call) const
{
    if (this->calls.contains(call))
        return this->calls[call]->incomingLine();
    return QString();
}
