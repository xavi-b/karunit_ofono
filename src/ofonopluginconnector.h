#ifndef OFONOPLUGINCONNECTOR_H
#define OFONOPLUGINCONNECTOR_H

#include <qofonomanager.h>
#include <qofonomodem.h>
#include <qofonovoicecallmanager.h>
#include <qofonovoicecall.h>
#include <QDebug>
#include "plugininterface.h"

class KU_Ofono_PluginConnector : public KU::PLUGIN::PluginConnector
{
    Q_OBJECT

    Q_PROPERTY(QStringList callsAliases READ callsAliases NOTIFY callsChanged)
public:
    KU_Ofono_PluginConnector(QObject* parent = nullptr);
    virtual void pluginSlot(QString const& signal, QVariantMap const& data) override;

    Q_INVOKABLE void call(QString const& number);
    Q_INVOKABLE void hangupAll();

    QStringList callsAliases() const;

    Q_INVOKABLE void    answer(QString const& call);
    Q_INVOKABLE void    hangup(QString const& call);
    Q_INVOKABLE QString state(QString const& call) const;
    Q_INVOKABLE QString lineIdentification(QString const& call) const;
    Q_INVOKABLE QString incomingLine(QString const& call) const;

signals:
    void log(QString const& log);
    void callSignal(QString const& number);
    void callsChanged();

private:
    void connectModem();
    void connectVoiceCallManager();

    QMap<QString, QOfonoVoiceCall*> calls;
    QOfonoManager*                  ofonoManager          = nullptr;
    QOfonoModem*                    ofonoModem            = nullptr;
    QOfonoVoiceCallManager*         ofonoVoiceCallManager = nullptr;
};

#endif // OFONOPLUGINCONNECTOR_H
