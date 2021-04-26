#ifndef CONTROLITEMEDITDELEGATEFRAMEOFLUMINAIREGROUP_H
#define CONTROLITEMEDITDELEGATEFRAMEOFLUMINAIREGROUP_H

#include <QFrame>
#include "domain/timelinecontrolitem.h"

namespace Ui {
    class ControlItemEditDialogFrameOfLuminaireGroup;
}

class MeshModel;

class ControlItemEditDialogFrameOfLuminaireGroup : public QFrame
{
    Q_OBJECT

public:
    explicit ControlItemEditDialogFrameOfLuminaireGroup(MeshModel *meshModel,
                                                        TimeLineControlItem *controlItem, QWidget *parent = 0);
    ~ControlItemEditDialogFrameOfLuminaireGroup();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_comboBox_activated(int);

    void on_radioLuxTypeFeedBack_clicked(bool);

    void on_radioLuxTypeNotFeedBack_clicked(bool);

    void on_buttonDone_clicked();

    void on_buttonCancel_clicked();

    void on_checkBindLuxSensorResultAsTrigPower_clicked(bool checked);

    void on_tabWidget_currentChanged(int);

private:
    Ui::ControlItemEditDialogFrameOfLuminaireGroup *ui;
    void setType(const QString &type);
    TimeLineControlItem *controlItem;
    MeshModel *m_meshModel;

    void setUiLuxSensorFeedBack(bool feedBack);

    void setControlItem(TimeLineControlItem *controlItem);

signals:
    void accept();
    void reject();
};

#endif // CONTROLITEMEDITDELEGATEFRAMEOFLUMINAIREGROUP_H
