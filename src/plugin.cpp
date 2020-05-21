#include "plugin.h"

void KU_Ofono_PluginConnector::pluginSlot(const QString& signal, const QVariantMap& data)
{
    if(signal == "dial")
    {
        if(!data["number"].isNull() && !data["number"].toString().isEmpty())
            emit callSignal(data["number"].toString());
    }
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
    return QIcon();
}

bool KU_Ofono_Plugin::initialize()
{
    if(QFontDatabase::addApplicationFont(":/karunit_ofono/FontAwesome") < 0)
        qWarning() << "FontAwesome cannot be loaded !";

    this->ofonoWidget = new OfonoWidget;

    this->pluginConnector = new KU_Ofono_PluginConnector;
    this->setPluginConnector(this->pluginConnector);
    connect(this->pluginConnector, &KU_Ofono_PluginConnector::callSignal, this->ofonoWidget, &OfonoWidget::call);
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

bool KU_Ofono_Plugin::loadSettings()
{
    return true;
}

bool KU_Ofono_Plugin::saveSettings() const
{
    return KU::Settings::instance()->status() == QSettings::NoError;
}
