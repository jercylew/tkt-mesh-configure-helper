#ifndef CONTROLITEMEDITDELEGATEFRAMEOFRELAYGROUP_H
#define CONTROLITEMEDITDELEGATEFRAMEOFRELAYGROUP_H

#include <QFrame>
#include "domain/timelinecontrolitem.h"
#include "model/meshmodel.h"

namespace Ui {
    class ControlItemEditDialogFrameOfRelayGroup;
}

class MeshModel;

class ControlItemEditDialogFrameOfRelayGroup : public QFrame
{
    Q_OBJECT

public:
    explicit ControlItemEditDialogFrameOfRelayGroup(MeshModel *meshModel, TimeLineControlItem *controlItem, QWidget *parent = 0);
    ~ControlItemEditDialogFrameOfRelayGroup();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_comboBox_activated(int);

    void on_buttonDone_clicked();

    void on_buttonCancel_clicked();

    void on_checkBindLuxSensorResultAsTrigPower_clicked(bool checked);

    void on_tabWidget_currentChanged(int);

    void on_btnAddANDGroup_clicked();

    void on_btnClearGroups_clicked();

private:
    Ui::ControlItemEditDialogFrameOfRelayGroup *ui;
    void setType(const QString &type);
    TimeLineControlItem *controlItem;
    MeshModel *m_meshModel;
    QVector<QVector<SensorDataComparision*>> m_vecSensorDataCmpORGroups;
    QVector<QWidget*> m_vecORCmpGroupVLayoutChildren;
    QMap<QString, int> m_mapSensorCmpCount;

    void setControlItem(TimeLineControlItem *controlItem);
    QString getCompositeSensorComparisionExpression();
    QString getAndCmpExpressionGroup(const QVector<SensorDataComparision*> &vecCmp);
    QString CmpOptr2String(QString strOptr);
    void initCompositeSensorControlState(TimeLineControlItem *controlItem);
    void addAndCmpGroupToView(const QVector<SensorDataComparision*> &vecCmp);

signals:
    void accept();
    void reject();
};

#endif // CONTROLITEMEDITDELEGATEFRAMEOFRELAYGROUP_H
