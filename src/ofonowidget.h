#ifndef OFONOWIDGET_H
#define OFONOWIDGET_H

#include <QWidget>
#include <qofono-qt5/qofonomanager.h>
#include <qofono-qt5/qofonomodem.h>
#include <qofono-qt5/qofonovoicecallmanager.h>
#include <qofono-qt5/qofonovoicecall.h>
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
