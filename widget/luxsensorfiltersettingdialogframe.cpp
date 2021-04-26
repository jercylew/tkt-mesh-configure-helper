#include "luxsensorfiltersettingdialogframe.h"
#include "ui_luxsensorfiltersettingdialogframe.h"
#include "model/meshmodel.h"
#include "utils/meshsavingbox.h"

LuxSensorFilterSettingDialogFrame::LuxSensorFilterSettingDialogFrame(MeshModel *meshModel, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LuxSensorFilterSettingDialogFrame)
{
    ui->setupUi(this);
    this->m_meshModel=meshModel;

    QList<QString> luxSensorIdList=m_meshModel->savingBox()->luxSensorListThatShouldBeFiltered();
    QString text;
    for(int i=0; i<luxSensorIdList.size(); i++)
    {
        text.push_back(luxSensorIdList.at(i));
        if(i!=(luxSensorIdList.size()-1))
        {
            text.push_back(",");
        }
    }
    ui->textLuxSensorIDs->setText(text);

    ui->combFilterCoefficient->setCurrentText(QString::number(m_meshModel->savingBox()->luxSensorFilterCoefficient()));
}

LuxSensorFilterSettingDialogFrame::~LuxSensorFilterSettingDialogFrame()
{
    delete ui;
}

void LuxSensorFilterSettingDialogFrame::on_buttonDialogDone_clicked()
{
    QList<QString> luxSensorIdList;
    QList<QString> strList=ui->textLuxSensorIDs->toPlainText().split(",");
    for(int i=0; i<strList.size(); i++)
    {
        if(strList.at(i).trimmed().isEmpty())
        {
            continue;
        }
        luxSensorIdList.push_back(strList.at(i));
    }
    m_meshModel->savingBox()->setLuxSensorListThatShouldBeFiltered(luxSensorIdList);
    m_meshModel->savingBox()->setLuxSensorFilterCoefficient(ui->combFilterCoefficient->currentText().toInt());
    emit accept();
}

void LuxSensorFilterSettingDialogFrame::on_buttonDialogCancel_clicked()
{
    emit reject();
}
