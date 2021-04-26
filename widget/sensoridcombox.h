#ifndef SENSORIDCOMBOX_H
#define SENSORIDCOMBOX_H

#include <QComboBox>
#include "domain/sensor.h"
#include <QSet>

class MeshModel;

class SensorIdCombox : public QComboBox
{
    Q_OBJECT
public:
    SensorIdCombox(QWidget *parent=0);

    void setText(const QString &text);

    QString text();

    void setMeshModel(MeshModel *meshModel);

    void focusOutEvent(QFocusEvent *e);

    void showPopup();

    void addAccessSensorType(Sensor::SensorType sensorType);

private:
   MeshModel *m_meshModel;

   void updateSensorList();

   QString m_sensorIdBeforePopup;

   QSet<Sensor::SensorType> m_accessSensorTypeSet;

   QList<QString> getFilteredAndSortedSensorIdList();

private slots:
   void doActivate(int index);
   void doHighlight(int index);

signals:
   void editingFinished();

};

#endif // SENSORIDLINEEDIT_H
