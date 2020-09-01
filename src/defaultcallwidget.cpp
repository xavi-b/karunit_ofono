#include "defaultcallwidget.h"

DefaultCallWidget::DefaultCallWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* layout = new QVBoxLayout;

    QHBoxLayout* topLayout = new QHBoxLayout;
    this->dialEdit = new QLineEdit;
    this->dialEdit->setPlaceholderText("Number to call");
    topLayout->addWidget(this->dialEdit, 1);
    QPushButton* eraseBtn = new QPushButton(QIcon::fromTheme("edit-clear"), "");
    connect(eraseBtn, &QPushButton::clicked, this, [=]()
    {
        this->dialEdit->setText(this->dialEdit->text().chopped(1));
    });
    topLayout->addWidget(eraseBtn);
    layout->addLayout(topLayout);

    QGridLayout* gridLayout = new QGridLayout;
    gridLayout->addWidget(this->addButton("1"), 0, 0);
    gridLayout->addWidget(this->addButton("2"), 0, 1);
    gridLayout->addWidget(this->addButton("3"), 0, 2);
    gridLayout->addWidget(this->addButton("4"), 1, 0);
    gridLayout->addWidget(this->addButton("5"), 1, 1);
    gridLayout->addWidget(this->addButton("6"), 1, 2);
    gridLayout->addWidget(this->addButton("7"), 2, 0);
    gridLayout->addWidget(this->addButton("8"), 2, 1);
    gridLayout->addWidget(this->addButton("9"), 2, 2);
    gridLayout->addWidget(this->addButton("+"), 3, 0);
    gridLayout->addWidget(this->addButton("0"), 3, 1);
    gridLayout->addWidget(this->addButton("#"), 3, 2);
    QPushButton* callBtn = new QPushButton(QIcon::fromTheme("call-start"), "");
    gridLayout->addWidget(callBtn, 5, 0);
    QPushButton* hangupAllBtn = new QPushButton(QIcon::fromTheme("call-stop"), "");
    gridLayout->addWidget(hangupAllBtn, 5, 2);
    layout->addLayout(gridLayout);

    this->setLayout(layout);

    connect(callBtn, &QPushButton::clicked, this, [=]()
    {
        emit call(this->dialEdit->text());
    });

    connect(hangupAllBtn, &QPushButton::clicked, this, &DefaultCallWidget::hangupAll);
}

QWidget* DefaultCallWidget::addButton(QString const& number)
{
    QPushButton* btn = new QPushButton(number);
    connect(btn, &QPushButton::clicked, this, [=]()
    {
        this->dialEdit->insert(number);
    });
    return btn;
}
