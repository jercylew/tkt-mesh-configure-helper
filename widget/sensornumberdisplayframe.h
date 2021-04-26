#ifndef SENSORNUMBERDISPLAYFRAME_H
#define SENSORNUMBERDISPLAYFRAME_H

#include <QFrame>
#include <QPropertyAnimation>

namespace Ui {
class SensorNumberDisplayFrame;
}

class SensorNumberDisplayFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SensorNumberDisplayFrame(const QString &typeText, const QString &m_sensorNotTranslatedText, QWidget *parent = 0);
    ~SensorNumberDisplayFrame();

    void changeEvent(QEvent *ev);
    void setNumber(int number);

    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);

    void animatedShowText();
    void animatedHideText();

private:
    Ui::SensorNumberDisplayFrame *ui;

private:
    QString m_typeText;
    QString m_sensorNotTranslatedText;
    QPropertyAnimation *m_propertyAnimation;
};

#endif // SENSORNUMBERDISPLAYFRAME_H
