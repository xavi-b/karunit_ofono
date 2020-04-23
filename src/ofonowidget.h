#ifndef OFONOWIDGET_H
#define OFONOWIDGET_H

#include <QWidget>
#include <qofonomanager.h>
#include <qofonomodem.h>
#include <qofonovoicecallmanager.h>
#include <qofonovoicecall.h>
#include <QTabWidget>
#include <QTabBar>
#include <QDebug>
#include "defaultcallwidget.h"
#include "voicecallwidget.h"

class OfonoWidget : public QWidget
{
    Q_OBJECT
private:
    void connectModem();
    void connectVoiceCallManager();

    QOfonoManager* ofonoManager = nullptr;
    QOfonoModem* ofonoModem = nullptr;
    QOfonoVoiceCallManager* ofonoVoiceCallManager = nullptr;
    QOfonoVoiceCall* ofonoVoiceCall = nullptr;
    QHash<QString, QWidget*> ofonoVoiceCallWidgets;
    DefaultCallWidget* defaultWidget;
    QTabWidget* tabWidget;

public:
    OfonoWidget(QWidget* parent = nullptr);

};

#endif // OFONOWIDGET_H
