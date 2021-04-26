#include "adboardcontroldialogframe.h"
#include "ui_adboardcontroldialogframe.h"
#include "domain/executer.h"
#include "domain/adboard.h"
#include "model/meshmodel.h"
#include "model/executersmodel.h"
#include "utils/commandtaskmanager.h"

ADBoardControlDialogFrame::ADBoardControlDialogFrame(MeshModel *meshModel, const QVariant &id, bool isGroup, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ADBoardControlDialogFrame)
{
    ui->setupUi(this);
    this->m_meshModel=meshModel;
    this->m_id=id;
    this->m_isGroup=isGroup;
    if(!isGroup)
    {
        Executer *executer=m_meshModel->executersModel()->getExecuterByExecuterId(this->m_id.toString());
        if(executer!=NULL && executer->typeText()==ADBoard::staticTypeText())
        {
            ADBoard *adBoard=static_cast<ADBoard*>(executer);
            ui->combDisplayMode->setCurrentText(QString::number(adBoard->displayMode()));
        }
    }
}

ADBoardControlDialogFrame::~ADBoardControlDialogFrame()
{
    delete ui;
}

void ADBoardControlDialogFrame::on_buttonDialogBottomClose_clicked()
{
    emit accept();
}

void ADBoardControlDialogFrame::on_buttonSetADBoardDisplayMode_clicked()
{
    int mode=ui->combDisplayMode->currentText().toInt();
    if(m_isGroup)
    {
        m_meshModel->commandTaskManager()->runSetGroupADBoardDisplayMode(m_id.toInt(), mode);
    }
    else
    {
        Executer *executer=m_meshModel->executersModel()->getExecuterByExecuterId(this->m_id.toString());
        if(executer==NULL)
        {
            return;
        }
        if(executer->getBluetoothAddress()==0)
        {
            return;
        }
        m_meshModel->commandTaskManager()->runSetADBoardDisplayMode(executer->getBluetoothAddress(), mode);
    }
}
