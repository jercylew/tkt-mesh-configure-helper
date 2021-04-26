#include "resetallnodedialogframe.h"
#include "ui_resetallnodedialogframe.h"
#include "model/meshmodel.h"
#include "model/executersmodel.h"
#include "utils/commandtaskmanager.h"

ResetAllNodeDialogFrame::ResetAllNodeDialogFrame(MeshModel *meshModel, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ResetAllNodeDialogFrame),
    m_meshModel(meshModel)
{
    ui->setupUi(this);
    currentIndex=0;

    QList<Executer *> lumList=m_meshModel->executersModel()->getAll();
    int lenLums=lumList.size();
    for(int i=0; i<lenLums; i++)
    {
        QList<quint8> groupList=lumList.at(i)->getGroupList();
        int lenGroups=groupList.size();
        for(int j=0; j<lenGroups; j++)
        {
            QPair<quint8, quint8> nodeAndGroupPair(lumList.at(i)->getBluetoothAddress(), groupList.at(j));
            pairList.push_back(nodeAndGroupPair);
        }
    }

    runTimer.setInterval(500); //500 ms
    connect(&runTimer, SIGNAL(timeout()), this, SLOT(doRunTimerTimeout()));
    runTimer.start();

    doRunTimerTimeout();

    ui->buttonDialogBottomClose->setCursor(Qt::PointingHandCursor);
}

ResetAllNodeDialogFrame::~ResetAllNodeDialogFrame()
{
    delete ui;
}

void ResetAllNodeDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void ResetAllNodeDialogFrame::on_buttonDialogBottomClose_clicked()
{
    runTimer.stop();
    emit reject();
}

void ResetAllNodeDialogFrame::doRunTimerTimeout()
{
    if(currentIndex>(pairList.size()-1))
    {
        ui->labelDialogProgress->setText("Finished!");
        runTimer.stop();
        QTimer::singleShot(500, this, SLOT(on_buttonDialogBottomClose_clicked()));
        return;
    }
    QPair<quint8, quint8> nodeAndGroupPair=pairList.at(currentIndex);
    QList<quint16> addrList;
    addrList.push_back(nodeAndGroupPair.first);
    m_meshModel->commandTaskManager()->runDeleteG1Command(addrList, nodeAndGroupPair.second);
    ui->labelDialogProgress->setText(QString("Progress: %1/%2").arg(currentIndex+1).arg(pairList.size()));
    currentIndex++;
}
