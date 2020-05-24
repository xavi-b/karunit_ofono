#include "ofonowidget.h"

void OfonoWidget::connectModem()
{
    connect(this->ofonoModem, &QOfonoModem::reportError, this, [=](QString const& errorString)
    {
        qDebug() << "reportError" << errorString;
    });
    connect(this->ofonoModem, &QOfonoModem::poweredChanged, this, [=](bool powered)
    {
        qDebug() << "poweredChanged" << powered;
        this->ofonoModem->setOnline(true);
    });
    connect(this->ofonoModem, &QOfonoModem::onlineChanged, this, [=](bool online)
    {
        qDebug() << "onlineChanged" << online;
        this->ofonoVoiceCallManager = new QOfonoVoiceCallManager(this);
        this->ofonoVoiceCallManager->setModemPath(this->ofonoModem->modemPath());
        connectVoiceCallManager();
    });
    qDebug() << "setPowered";
    //this->ofonoModem->setPowered(true);
    QDBusMessage message = QDBusMessage::createMethodCall("org.ofono",
                                                          this->ofonoModem->modemPath(),
                                                          "org.ofono.Modem",
                                                          "SetProperty");
    QList<QVariant> arguments;
    arguments << QString("Powered");
    arguments << QVariant::fromValue(QDBusVariant(true));

    message.setArguments(arguments);

    qDebug() << QDBusConnection::systemBus().send(message);
}

void OfonoWidget::connectVoiceCallManager()
{
    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::callAdded, this, [=](const QString &call)
    {
        qDebug() << "callAdded" << call;
        auto ofonoVoiceCallWidget = new VoiceCallWidget(call, this);
        auto it = this->ofonoVoiceCallWidgets.find(call);
        if(it == this->ofonoVoiceCallWidgets.end())
        {
            this->ofonoVoiceCallWidgets.insert(call, ofonoVoiceCallWidget);
            this->tabWidget->addTab(ofonoVoiceCallWidget, ofonoVoiceCallWidget->incomingLine());
        }
    });
    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::callRemoved, this, [=](const QString &call)
    {
        qDebug() << "callRemoved" << call;
        auto it = this->ofonoVoiceCallWidgets.find(call);
        if(it != this->ofonoVoiceCallWidgets.end())
        {
            this->ofonoVoiceCallWidgets.remove(call);
            this->tabWidget->removeTab(this->tabWidget->indexOf(it.value()));
        }
    });
    connect(this->ofonoVoiceCallManager, &QOfonoVoiceCallManager::dialComplete, this, [=](bool status)
    {
        qDebug() << "dialComplete" << status << this->ofonoVoiceCallManager->errorMessage();
    });
}

OfonoWidget::OfonoWidget(QWidget *parent)
    : QWidget(parent)
{
    this->setMinimumSize(640, 400);

    this->setFont(QFont("Font Awesome 5 Free"));

    QVBoxLayout* layout = new QVBoxLayout;

    this->tabWidget = new QTabWidget;
    layout->addWidget(this->tabWidget);
    this->defaultWidget = new DefaultCallWidget;
    this->tabWidget->addTab(this->defaultWidget, "\uf015");

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
            qDebug() << "no modem";
            return;
        }

        qDebug() << "availableChanged" << available << this->ofonoManager->modems().at(0);
        this->ofonoModem = new QOfonoModem(this);
        this->ofonoModem->setModemPath(this->ofonoManager->modems().at(0));
        connectModem();
    });
    connect(this->ofonoManager, &QOfonoManager::modemAdded, this, [=](const QString &modem)
    {
        qDebug() << "modemAdded" << modem;
    });
    connect(this->ofonoManager, &QOfonoManager::modemRemoved, this, [=](const QString &modem)
    {
        qDebug() << "modemRemoved" << modem;
    });
    connect(this->ofonoManager, &QOfonoManager::defaultModemChanged, this, [=](const QString &modem)
    {
        qDebug() << "defaultModemChanged" << modem;
    });
}

void OfonoWidget::call(QString const& number)
{
    if(this->ofonoVoiceCallManager != nullptr)
        this->ofonoVoiceCallManager->dial(number, "enabled");
}

