#ifndef VOICECALLWIDGET_H
#define VOICECALLWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <qofonovoicecall.h>

class VoiceCallWidget : public QWidget
{
    Q_OBJECT
private:
    QOfonoVoiceCall* ofonoVoiceCall = nullptr;
    QPushButton* answerBtn;
    QPushButton* hangupBtn;
    QLabel* numberLabel;
    QLabel* statusLabel;

public:
    VoiceCallWidget(QString const& call, QWidget *parent = nullptr);

    QString incomingLine() const;

};

#endif // VOICECALLWIDGET_H
