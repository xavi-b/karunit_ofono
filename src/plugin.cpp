#include "plugin.h"

void KU_Ofono_PluginConnector::pluginSlot(const QString& signal, const QVariantMap& data)
{
    if(signal == "dial")
    {
        if(!data["number"].isNull() && !data["number"].toString().isEmpty())
            emit callSignal(data["number"].toString());
    }
}

KU_Ofono_Plugin::KU_Ofono_Plugin()
{
    this->pluginConnector = new KU_Ofono_PluginConnector;
    this->setPluginConnector(this->pluginConnector);
}

KU_Ofono_Plugin::~KU_Ofono_Plugin()
{
    if(this->pluginConnector != nullptr)
        delete this->pluginConnector;
}

QString KU_Ofono_Plugin::name() const
{
    return "Ofono";
}

QString KU_Ofono_Plugin::id() const
{
    return "ofono.me";
}

KU::PLUGIN::PluginVersion KU_Ofono_Plugin::version() const
{
    return { 1, 0, 0 };
}

QString KU_Ofono_Plugin::license() const
{
    return "LGPL";
}

QIcon KU_Ofono_Plugin::icon() const
{
    return QIcon::fromTheme("modem");
}

bool KU_Ofono_Plugin::initialize()
{
    this->ofonoWidget = new OfonoWidget;

    connect(this->pluginConnector, &KU_Ofono_PluginConnector::callSignal, this->ofonoWidget, &OfonoWidget::call);
    connect(this->ofonoWidget, &OfonoWidget::log, this->pluginConnector, &KU::PLUGIN::PluginConnector::log);

    return true;
}

bool KU_Ofono_Plugin::stop()
{
    return true;
}

QWidget* KU_Ofono_Plugin::createWidget()
{
    return this->ofonoWidget;
}

QWidget* KU_Ofono_Plugin::createSettingsWidget()
{
    return new QLabel("Ofono");
}

QWidget* KU_Ofono_Plugin::createAboutWidget()
{
    return nullptr;
}

bool KU_Ofono_Plugin::loadSettings()
{
    return true;
}

bool KU_Ofono_Plugin::saveSettings() const
{
    return KU::Settings::instance()->status() == QSettings::NoError;
}
