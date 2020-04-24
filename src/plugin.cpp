#include "plugin.h"

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
    if(QFontDatabase::addApplicationFont(":/FontAwesome") < 0)
        qWarning() << "FontAwesome cannot be loaded !";

    return true;
}

bool KU_Ofono_Plugin::stop()
{
    return true;
}

QWidget* KU_Ofono_Plugin::createWidget()
{
    OfonoWidget* widget = new OfonoWidget;
    return widget;
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
