#ifndef ALLOCSWITCHKEYGROUPFRAME_H
#define ALLOCSWITCHKEYGROUPFRAME_H

#include <QFrame>
#include <QPair>
#include <QComboBox>

namespace Ui {
class AllocSwitchKeyGroupFrame;
}

class MeshModel;

class AllocSwitchKeyGroupFrame : public QFrame
{
    Q_OBJECT

public:
    enum KeyCmdType
    {
        Grouping = 0x01,
        NoOperation = -1
    };

    explicit AllocSwitchKeyGroupFrame(MeshModel *meshModel, const QString &sensorId, QWidget *parent = 0);
    ~AllocSwitchKeyGroupFrame();    

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

signals:
    void accept();
    void reject();

private:
    void setUpKeyFrame();
    void fillKeyData();
    void fillGroupingData(QString strGroupName, quint8 groupId);
    void fillFuncData(QString strFuncName, KeyCmdType cmdCode);
    void fillPredefinedCmds();
    void setDefaultFuncCmbox();

    Ui::AllocSwitchKeyGroupFrame *ui;
    MeshModel *m_meshModel;
    QString m_sensorId;
    QMap<QString, KeyCmdType> m_mapCtrlCmd;
};

#endif // ALLOCSWITCHKEYGROUPFRAME_H
