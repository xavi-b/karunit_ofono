#ifndef OFONOPLUGIN_H
#define OFONOPLUGIN_H

#include <QtPlugin>
#include <QIcon>
#include <QDebug>
#include <QLabel>
#include <QDateTime>
#include <QFontDatabase>
#include "plugininterface.h"
#include "settings.h"
#include "ofonowidget.h"

class KU_Ofono_PluginConnector : public KU::PLUGIN::PluginConnector
{
    Q_OBJECT
public:
    virtual void pluginSlot(QString const& signal, QVariantMap const& data) override;

signals:
    void callSignal(QString const& number);
};

class KU_Ofono_Plugin : public QObject, public KU::PLUGIN::PluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "xavi-b.karunit.PluginInterface")
    Q_INTERFACES(KU::PLUGIN::PluginInterface)

public:
    virtual QString name() const override;
    virtual QString id() const override;
    virtual KU::PLUGIN::PluginVersion version() const override;
    virtual QString license() const override;
    virtual QIcon icon() const override;
    virtual bool initialize() override;
    virtual bool stop() override;

    virtual QWidget* createWidget() override;
    virtual QWidget* createSettingsWidget() override;
    virtual bool loadSettings() override;
    virtual bool saveSettings() const override;

private:
    OfonoWidget* ofonoWidget = nullptr;
    KU_Ofono_PluginConnector* pluginConnector = nullptr;
};


#endif // OFONOPLUGIN_H
