#ifndef DEFAULTCALLWIDGET_H
#define DEFAULTCALLWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>

class DefaultCallWidget : public QWidget
{
    Q_OBJECT
private:
    QLineEdit* dialEdit;

    QWidget* addButton(const QString& number);

public:
    DefaultCallWidget(QWidget* parent = nullptr);

signals:
    void call(QString const& number);
    void hangupAll();
};

#endif // DEFAULTCALLWIDGET_H
