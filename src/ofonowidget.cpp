#include "ofonowidget.h"

void OfonoWidget::connectModem()
{
    connect(this->ofonoModem, &QOfonoModem::reportError, this, [=](QString const& errorString)
    {
        emit log("reportError: " + errorString);
    });
    connect(this->ofonoModem, &QOfonoModem::poweredChanged, this, [=](bool powered)
    {
        emit log(QString("poweredChanged: %1").arg(powered));
        this->ofonoModem->setOnline(true);
    });
    connect(this->ofonoModem, &QOfonoModem::onlineChanged, this, [=](bool online)
    {
        emit log(QString("onlineChanged: %1").arg(online));
        this->ofonoVoiceCallManager = new QOfonoVoiceCallManager(this);
        this->ofonoVoiceCallManager->setModemPath(this->ofonoModem->modemPath());
        connectVoiceCallManager();
    });
    emit log("setPowered");
    //this->ofonoModem->setPowered(true);
    QDBusMessage message = QDBusMessage::createMethodCall("org.ofono",
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

void OfonoWidget::connectVoiceCallManager()
{
    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::callAdded, this, [=](const QString &call)
    {
        emit log("callAdded: " + call);
        auto ofonoVoiceCallWidget = new VoiceCallWidget(call, this);
        auto it = this->ofonoVoiceCallWidgets.find(call);
        if(it == this->ofonoVoiceCallWidgets.end())
        {
            QString address = call.split('/').at(5);
            address.replace("dev_", "");
            address.replace("_", ":");
            QProcess* recordProcess = new QProcess(this);
            connect(recordProcess, qOverload<int, QProcess::ExitStatus>(&QProcess::finished), this, [this](int exitCode, QProcess::ExitStatus exitStatus){
                emit log("Record process finished: " + QString::number(exitCode) + " " + QString::number(exitStatus == QProcess::ExitStatus::NormalExit));
            });
            connect(recordProcess, &QProcess::readyReadStandardOutput, this, [this, recordProcess](){
                emit log("Record process output: " + recordProcess->readAllStandardOutput());
            });
            connect(recordProcess, &QProcess::readyReadStandardError, this, [this, recordProcess](){
                emit log("Record process error: " + recordProcess->readAllStandardError());
            });
            recordProcess->start("/bin/sh -c \"LIBASOUND_THREAD_SAFE=0 /usr/bin/arecord -f s16_le | /usr/bin/aplay -D bluealsa:DEV=" + address + ",PROFILE=sco\"");
            emit log("Record process started for: " + address);
            if(!recordProcess->waitForStarted())
               emit log("Record process failed: " + recordProcess->errorString() + " " + QString::number(recordProcess->exitCode()) + " " + recordProcess->readAllStandardError());

            this->ofonoVoiceCallWidgets.insert(call, ofonoVoiceCallWidget);
            this->tabWidget->addTab(ofonoVoiceCallWidget, ofonoVoiceCallWidget->incomingLine());
        }
    });
    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::callRemoved, this, [=](const QString &call)
    {
        emit log("callRemoved: " + call);
        auto it = this->ofonoVoiceCallWidgets.find(call);
        if(it != this->ofonoVoiceCallWidgets.end())
        {
            this->ofonoVoiceCallWidgets.remove(call);
            this->tabWidget->removeTab(this->tabWidget->indexOf(it.value()));
        }
    });
    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::dialComplete, this, [=](bool status)
    {
        emit log(QString("dialComplete: [%1] ").arg(status) + this->ofonoVoiceCallManager->errorMessage());
    });
}

OfonoWidget::OfonoWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;

    this->tabWidget = new QTabWidget;
    layout->addWidget(this->tabWidget);
    this->defaultWidget = new DefaultCallWidget;
    this->tabWidget->addTab(this->defaultWidget, QIcon::fromTheme("go-home"), "");

    connect(this->defaultWidget, &DefaultCallWidget::call, this, &OfonoWidget::call);
    connect(this->defaultWidget, &DefaultCallWidget::hangupAll, this, [=]()
    {
        if(this->ofonoVoiceCallManager != nullptr)
            this->ofonoVoiceCallManager->hangupAll();
    });

    this->setLayout(layout);

    this->ofonoManager = new QOfonoManager(this);

    connect(this->ofonoManager, &QOfonoManager::availableChanged, this, [=](bool available)
    {
        if(this->ofonoManager->modems().size() == 0)
        {
            emit log("no modem");
            return;
        }

        emit log(QString("availableChanged [%1] ").arg(available) + this->ofonoManager->modems().at(0));
        this->ofonoModem = new QOfonoModem(this);
        this->ofonoModem->setModemPath(this->ofonoManager->modems().at(0));
        connectModem();
    });
    connect(this->ofonoManager, &QOfonoManager::modemAdded, this, [=](const QString &modem)
    {
        emit log("modemAdded: " + modem);
    });
    connect(this->ofonoManager, &QOfonoManager::modemRemoved, this, [=](const QString &modem)
    {
        emit log("modemRemoved: " + modem);
    });
    connect(this->ofonoManager, &QOfonoManager::defaultModemChanged, this, [=](const QString &modem)
    {
        emit log("defaultModemChanged: " + modem);
    });
}

void OfonoWidget::call(QString const& number)
{
    if(this->ofonoVoiceCallManager != nullptr)
        this->ofonoVoiceCallManager->dial(number, "enabled");
}

