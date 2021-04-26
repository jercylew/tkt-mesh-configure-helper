#ifndef SENSORFRAME_H
#define SENSORFRAME_H

#include <QFrame>
#include <QLabel>

namespace Ui {
class SensorFrame;
}

class SensorFrame : public QFrame
{
    Q_OBJECT

public:
    explicit SensorFrame(QWidget *parent = 0);
    ~SensorFrame();

    QLabel *getIconLabel();
    void setName(const QString &name);
    void setId(const QString &sensorId);
    void setStateText(const QString &stateText);
    void resetStyle();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::SensorFrame *ui;
};

#endif // SENSORFRAME_H
