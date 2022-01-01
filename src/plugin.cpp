#include "plugin.h"
#include <QQmlEngine>

QString KU_Ofono_Plugin::name() const
{
    return "Ofono";
}

QString KU_Ofono_Plugin::id() const
{
    return "karunit_ofono";
}

KU::PLUGIN::PluginVersion KU_Ofono_Plugin::version() const
{
    return {1, 0, 0};
}

QString KU_Ofono_Plugin::license() const
{
    return "LGPL";
}

QString KU_Ofono_Plugin::icon() const
{
    return "modem";
}

bool KU_Ofono_Plugin::initialize()
{
    qmlRegisterSingletonInstance("KarunitPlugins", 1, 0, "KUPOfonoPluginConnector", this->pluginConnector);
    this->getPluginConnector()->setup();

    return true;
}

bool KU_Ofono_Plugin::stop()
{
    return true;
}

bool KU_Ofono_Plugin::loadSettings()
{
    return true;
}

bool KU_Ofono_Plugin::saveSettings() const
{
    return KU::Settings::instance()->status() == QSettings::NoError;
}

KU_Ofono_PluginConnector* KU_Ofono_Plugin::getPluginConnector()
{
    if (this->pluginConnector == nullptr)
        this->pluginConnector = new KU_Ofono_PluginConnector;
    return qobject_cast<KU_Ofono_PluginConnector*>(this->pluginConnector);
}
