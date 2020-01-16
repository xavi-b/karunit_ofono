#include "voicecallwidget.h"

VoiceCallWidget::VoiceCallWidget(QString const& call, QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;
    this->answerBtn = new QPushButton("Answer");
    layout->addWidget(this->answerBtn);
    this->hangupBtn = new QPushButton("Hang Up");
    layout->addWidget(this->hangupBtn);
    this->numberLabel = new QLabel("Number");
    layout->addWidget(this->numberLabel);
    this->statusLabel = new QLabel("Status");
    layout->addWidget(this->statusLabel);
    this->setLayout(layout);

    this->ofonoVoiceCall = new QOfonoVoiceCall(this);
    this->ofonoVoiceCall->setVoiceCallPath(call);

    this->numberLabel->setText(this->ofonoVoiceCall->lineIdentification());
    connect(this->ofonoVoiceCall, &QOfonoVoiceCall::lineIdentificationChanged, this->numberLabel, &QLabel::setText);
    this->statusLabel->setText(this->ofonoVoiceCall->state());
    connect(this->ofonoVoiceCall, &QOfonoVoiceCall::stateChanged, this->statusLabel, &QLabel::setText);

    connect(this->answerBtn, &QPushButton::clicked, this, [=]()
    {
        this->ofonoVoiceCall->answer();
    });
    connect(this->hangupBtn, &QPushButton::clicked, this, [=]()
    {
        this->ofonoVoiceCall->hangup();
    });
}

QString VoiceCallWidget::incomingLine() const
{
    return this->ofonoVoiceCall->incomingLine();
}
