#include "renamedialogframe.h"
#include "ui_renamedialogframe.h"
#include "db/meshdbmanager.h"
#include "model/meshmodel.h"
#include "model/executersmodel.h"
#include "model/sensorsmodel.h"
#include "db/meshdbcaller.h"

RenameDialogFrame::RenameDialogFrame(MeshModel *meshModel, const QString &type, const QList<QString> &idList, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::RenameDialogFrame),
    m_meshModel(meshModel)
{
    ui->setupUi(this);
    this->type=type;
    this->idList=idList;
    ui->tableWidget->setColumnWidth(0, 150);

    ui->tableWidget->setRowCount(idList.size());
    if(type=="light")
    {
        QList<int> numberList;
        int len=idList.size();
        for(int i=0; i<len; i++)
        {
            numberList.push_back(findNumber(m_meshModel->executersModel()->getNodeName(idList.at(i))).toInt());
        }
        qSort(numberList);
        ui->spinAutoDetected->setValue(numberList.last());
    }
    else if(type=="sensor")
    {
        QList<int> numberList;
        int len=idList.size();
        for(int i=0; i<len; i++)
        {
            numberList.push_back(findNumber(m_meshModel->sensorsModel()->getNodeName(idList.at(i))).toInt());
        }
        qSort(numberList);
        ui->spinAutoDetected->setValue(numberList.last());
    }

    on_combNameTemplate_currentTextChanged(ui->combNameTemplate->currentText());
    ui->combNameTemplate->setFocus();

    ui->buttonDone->setCursor(Qt::PointingHandCursor);
    ui->buttonCancel->setCursor(Qt::PointingHandCursor);
}

RenameDialogFrame::~RenameDialogFrame()
{
    delete ui;
}

void RenameDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void RenameDialogFrame::on_buttonDone_clicked()
{
    int number=0;
    if(ui->radioFixedNumber->isChecked())
    {
        number=ui->spinFixedNumber->value();
    }
    else
    {
        number=ui->spinAutoDetected->value();
    }
    if(type=="light")
    {
        QMap<QString, QPair<QString, QString> > *nameMap=new QMap<QString, QPair<QString, QString> >;
        int len=idList.size();
        for(int i=0; i<len; i++)
        {
            QString sharp=findSharp(ui->combNameTemplate->currentText());
            QString numSection=QString("%1").arg(number, sharp.length(), 10, QLatin1Char('0'));
            QString newName;
            if(sharp.isEmpty())
            {
                newName=ui->combNameTemplate->currentText();
            }
            else
            {
                newName=ui->combNameTemplate->currentText().replace(sharp, numSection);
            }
            QString id=idList.at(i);
            Executer *executer=m_meshModel->executersModel()->getExecuter(id);
            if(executer==NULL)
            {
                continue;
            }
            nameMap->insert(id, QPair<QString, QString>(executer->typeText(), newName));
            number++;
        }
        m_meshModel->dbManager()->dbCaller()->setExecuterNodesName(nameMap);
    }
    else if(type=="sensor")
    {
        QMap<QString, QPair<QString, QString> > *nameMap=new QMap<QString, QPair<QString, QString> >;
        int len=idList.size();
        for(int i=0; i<len; i++)
        {
            QString sharp=findSharp(ui->combNameTemplate->currentText());
            QString numSection=QString("%1").arg(number, sharp.length(), 10, QLatin1Char('0'));
            QString newName;
            if(sharp.isEmpty())
            {
                newName=ui->combNameTemplate->currentText();
            }
            else
            {
                newName=ui->combNameTemplate->currentText().replace(sharp, numSection);
            }
            QString id=idList.at(i);
            Sensor *sensor=m_meshModel->sensorsModel()->getSensorBySensorId(id);
            if(sensor==NULL)
            {
                continue;
            }
            nameMap->insert(id, QPair<QString, QString>(sensor->typeText(), newName));
            number++;
        }
        m_meshModel->dbManager()->dbCaller()->setSensorNodesName(nameMap);
    }
    emit accept();
}

void RenameDialogFrame::on_buttonCancel_clicked()
{
    emit reject();
}

void RenameDialogFrame::on_combNameTemplate_currentTextChanged(const QString &text)
{
    int number=0;
    if(ui->radioFixedNumber->isChecked())
    {
        number=ui->spinFixedNumber->value();
    }
    else
    {
        number=ui->spinAutoDetected->value();
    }
    if(type=="light")
    {
        int len=idList.size();
        for(int i=0; i<len; i++)
        {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(m_meshModel->executersModel()->getNodeName(idList.at(i))));
            QString sharp=findSharp(ui->combNameTemplate->currentText());
            QString numSection=QString("%1").arg(number, sharp.length(), 10, QLatin1Char('0'));
            QString newName;
            if(sharp.isEmpty())
            {
                newName=ui->combNameTemplate->currentText();
            }
            else
            {
                newName=ui->combNameTemplate->currentText().replace(sharp, numSection);
            }
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(newName));
            number++;
        }
    }
    else if(type=="sensor")
    {
        int len=idList.size();
        for(int i=0; i<len; i++)
        {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(m_meshModel->sensorsModel()->getNodeName(idList.at(i))));
            QString sharp=findSharp(ui->combNameTemplate->currentText());
            QString numSection=QString("%1").arg(number, sharp.length(), 10, QLatin1Char('0'));
            QString newName;
            if(sharp.isEmpty())
            {
                newName=ui->combNameTemplate->currentText();
            }
            else
            {
                newName=ui->combNameTemplate->currentText().replace(sharp, numSection);
            }
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(newName));
            number++;
        }
    }
}

QString RenameDialogFrame::findSharp(const QString &text)
{
    QRegExp exp("(\\#+)");
    if(exp.indexIn(text)!=-1)
    {
        return exp.cap(1);
    }
    return QString();
}

QString RenameDialogFrame::findNumber(const QString &text)
{
    QRegExp exp("(\\d+)");
    if(exp.indexIn(text)!=-1)
    {
        return exp.cap(1);
    }
    return QString();
}

void RenameDialogFrame::on_radioFixedNumber_clicked(bool checked)
{
    on_combNameTemplate_currentTextChanged(ui->combNameTemplate->currentText());
}

void RenameDialogFrame::on_radioAutoDetected_clicked(bool checked)
{
    on_combNameTemplate_currentTextChanged(ui->combNameTemplate->currentText());
}

void RenameDialogFrame::on_spinFixedNumber_valueChanged(int arg1)
{
    on_combNameTemplate_currentTextChanged(ui->combNameTemplate->currentText());
}

void RenameDialogFrame::on_spinAutoDetected_valueChanged(int arg1)
{
    on_combNameTemplate_currentTextChanged(ui->combNameTemplate->currentText());
}
