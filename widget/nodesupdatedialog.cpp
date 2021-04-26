#include "nodesupdatedialog.h"
#include "ui_nodesupdatedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include "addtktmeshdialog.h"
#include <QTcpSocket>
#include <QtConcurrent>
#include "domain/tktmesh.h"
#include "utils/tools.h"
#include "globalsavingbox.h"
#include "utils/updatethreadworker.h"

bool NodesUpdateDialog__asyncUpdateNode(const QString &ip, int waitMsec, const QString &updateFilePath)
{
    qDebug()<<"Updating ...";
    QHostAddress address(ip);
    QTcpSocket tcpSocket;
    tcpSocket.connectToHost(address, 5199);
    bool ok=tcpSocket.waitForConnected(waitMsec);
    if(!ok)
    {
        return false;
    }
    tcpSocket.waitForReadyRead(waitMsec);
    QByteArray data=tcpSocket.readAll();
    qDebug()<<"READ: "<<data;
    if(!data.startsWith("{"))
    {
        return false;
    }
    QJsonObject tktMeshJson=Tools::getJsonObjectFromDataArray(data);
    if(tktMeshJson.isEmpty())
    {
        return false;
    }
    if(tktMeshJson.value("topic").toString()!="description")
    {
        return false;
    }
    QJsonObject dataObj=tktMeshJson.value("data").toObject();
    if(dataObj.isEmpty())
    {
        return false;
    }
    QString hostID=dataObj.value("host_id").toString();
    if(hostID.isEmpty())
    {
        return false;
    }
    QFile file(updateFilePath);
    if(!file.open(QFile::ReadOnly))
    {
        return false;
    }
    tcpSocket.write(QString::number(file.size()).toLatin1());
    tcpSocket.write("\n");
    while(!file.atEnd())
    {
        QByteArray data=file.read(100*1024);
        tcpSocket.write(data);
        tcpSocket.flush();
        tcpSocket.waitForBytesWritten(10*1000);
    }
    file.close();
    tcpSocket.waitForReadyRead(10*1000);
    tcpSocket.close();
    return true;
}

NodesUpdateDialog::NodesUpdateDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NodesUpdateDialog)
{
    ui->setupUi(this);
    ui->lineUpdateFile->setReadOnly(true);
    m_idleTaskNumber=4;

    m_tktMeshesUpdateModel=new TKTMeshesUpdateModel(this);
    m_tktMeshUpdateTableViewModel=new TKTMeshUpdateTableViewModel(m_tktMeshesUpdateModel, this);
    connect(m_tktMeshesUpdateModel, SIGNAL(scanProgress(int,int)),
            this, SLOT(doScanProgress(int,int)));
    ui->tableTKTMeshUpdate->setModel(m_tktMeshUpdateTableViewModel);

    ui->tableTKTMeshUpdate->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableTKTMeshUpdate->verticalHeader()->setVisible( true );
    ui->tableTKTMeshUpdate->setSelectionMode(QTableView::ExtendedSelection);
    ui->tableTKTMeshUpdate->setSelectionBehavior(QTableView::SelectRows);

    ui->tableTKTMeshUpdate->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->tableTKTMeshUpdate->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    ui->tableTKTMeshUpdate->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    ui->tableTKTMeshUpdate->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    ui->tableTKTMeshUpdate->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->tableTKTMeshUpdate->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Fixed);

    ui->tableTKTMeshUpdate->setColumnWidth(0, 200);
    ui->tableTKTMeshUpdate->setColumnWidth(1, 200);
    ui->tableTKTMeshUpdate->setColumnWidth(2, 200);
    ui->tableTKTMeshUpdate->setColumnWidth(3, 200);
    ui->tableTKTMeshUpdate->setColumnWidth(5, 200);

    ui->lineUpdateFile->setText(GlobalSavingBox::getInstance()->getValue("LAST_UPDATE_FILE"));

    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);

    Qt::WindowFlags flags=this->windowFlags();
    flags |= Qt::WindowMinimizeButtonHint;
    flags |= Qt::WindowMaximizeButtonHint;
    this->setWindowFlags(flags);
}

NodesUpdateDialog::~NodesUpdateDialog()
{
    delete ui;
}

void NodesUpdateDialog::on_buttonSelectUpdateFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select Update File"),
                                                      GlobalSavingBox::getInstance()->getValue("LAST_UPDATE_FILE"),
                                                      tr("Update File (*.update)"));
    if(fileName.isEmpty())
    {
        return;
    }
    ui->lineUpdateFile->setText(fileName);
    GlobalSavingBox::getInstance()->setValue("LAST_UPDATE_FILE", fileName);
}

void NodesUpdateDialog::on_buttonUpdate_clicked()
{
    if(ui->lineUpdateFile->text().isEmpty())
    {
        QMessageBox::about(this, tr("Error"), tr("Update File cannot be empty!"));
        return;
    }
    if(!QFile(ui->lineUpdateFile->text()).exists())
    {
        QMessageBox::about(this, tr("Error"), tr("Update File is not existed!"));
        return;
    }
    if(QMessageBox::question(this, tr("Update"), tr("Are you sure to update?"))!=QMessageBox::Yes)
    {
        return;
    }
    QFile file(ui->lineUpdateFile->text());
    if(!file.open(QFile::ReadOnly))
    {
        QMessageBox::about(this, tr("Error"), tr("Cannot open Update File!"));
        return;
    }
    QByteArray header=file.read(80);
    file.close();
    QString packagePlatform=QString::fromLatin1(header).toUpper().trimmed();
    QModelIndexList list=ui->tableTKTMeshUpdate->selectionModel()->selectedRows();
    for(int i=0; i<list.size();  i++)
    {
        QString hostId=list.at(i).data(Qt::UserRole).toString();
        TKTMesh *tktMesh=m_tktMeshesUpdateModel->getByID(hostId);
        if(tktMesh==NULL)
        {
            continue;
        }
        if(tktMesh->platform()!=packagePlatform)
        {
            tktMesh->setUpdateProgress(tr("Package Error"));
            continue;
        }
        tktMesh->setUpdateProgress(tr("Waiting ..."));
        m_updateHostIDList.push_back(hostId);
        startUpdateTasks();
    }
    m_tktMeshUpdateTableViewModel->doDataChanged();
}

void NodesUpdateDialog::on_buttonBottomClose_clicked()
{
    this->accept();
}

void NodesUpdateDialog::doScanProgress(int total, int finished)
{
    if(total==finished)
    {
        ui->labelStatusText->setText(tr("扫描完成, 请选中下面的节点开始升级!"));
        ui->buttonScanTKTMesh->setEnabled(true);
        ui->buttonAddTKTMesh->setEnabled(true);
    }
    else
    {
        ui->labelStatusText->setText(tr("扫描进度: %1 / %2").arg(finished).arg(total));
    }
}

void NodesUpdateDialog::on_buttonScanTKTMesh_clicked()
{
    ui->buttonScanTKTMesh->setEnabled(false);
    ui->buttonAddTKTMesh->setEnabled(false);
    ui->labelStatusText->setText("扫描中...");
    QHostAddress address1(ui->lineFromAddress->text());
    QHostAddress address2(ui->lineToAddress->text());
    m_tktMeshesUpdateModel->scanTKTMesh(address1, address2, ui->spinWaitMsec->value(), true);
}

void NodesUpdateDialog::on_buttonAddTKTMesh_clicked()
{
    AddTKTMeshDialog dialog(this);
    if(dialog.exec()!=QDialog::Accepted)
    {
        return;
    }
    QHostAddress address(dialog.getIP());
    if(address.isNull())
    {
        return;
    }
    m_tktMeshesUpdateModel->scanTKTMesh(address, address, ui->spinWaitMsec->value(), false);
}

void NodesUpdateDialog::doUpdateNodeFinished()
{
    QFutureWatcher<bool> *watcher=static_cast<QFutureWatcher<bool>*>(sender());
    QString hostId=watcher->property("host_id").toString();
    TKTMesh *tktMesh=m_tktMeshesUpdateModel->getByID(hostId);
    if(tktMesh)
    {
        bool ok=watcher->result();
        if(ok)
        {
            tktMesh->setUpdateProgress("Success");
        }
        else
        {
            tktMesh->setUpdateProgress("Failed");
        }
        m_tktMeshUpdateTableViewModel->doDataChanged();
    }
    watcher->deleteLater();
}

void NodesUpdateDialog::doUpdateWorkStarted(const QString &hostId)
{
    TKTMesh *tktMesh=m_tktMeshesUpdateModel->getByID(hostId);
    if(tktMesh!=NULL)
    {
        tktMesh->setUpdateProgress(tr("Updating ..."));
        m_tktMeshUpdateTableViewModel->doDataChanged();
    }
}

void NodesUpdateDialog::doUpdateWorkFinished(const QString &hostId, bool ok)
{
    TKTMesh *tktMesh=m_tktMeshesUpdateModel->getByID(hostId);
    if(tktMesh!=NULL)
    {
        tktMesh->setUpdateProgress(ok?tr("Success"):tr("Failed"));
        m_tktMeshUpdateTableViewModel->doDataChanged();
    }
    m_idleTaskNumber++;
    startUpdateTasks();
    UpdateThreadWorker *worker=static_cast<UpdateThreadWorker*>(sender());
    if(worker)
    {
        worker->deleteLater();
    }
}

void NodesUpdateDialog::doUpdateWorkProgressReport(const QString &hostId, int progress)
{
    TKTMesh *tktMesh=m_tktMeshesUpdateModel->getByID(hostId);
    if(tktMesh!=NULL)
    {
        tktMesh->setUpdateProgress(QString("%1(%)").arg(progress));
        m_tktMeshUpdateTableViewModel->doDataChanged();
    }
}

void NodesUpdateDialog::startUpdateTasks()
{
    while((m_idleTaskNumber>0) && (!m_updateHostIDList.isEmpty()))
    {
        QString hostId=m_updateHostIDList.takeFirst();
        TKTMesh *tktMesh=m_tktMeshesUpdateModel->getByID(hostId);
        if(tktMesh==NULL)
        {
            continue;
        }
        QThread *workThread=new QThread;
        UpdateThreadWorker *worker=new UpdateThreadWorker(hostId, tktMesh->hostPeerIP(), ui->lineUpdateFile->text(), workThread);
        worker->moveToThread(workThread);
        connect(workThread, SIGNAL(started()), worker, SLOT(startUpdate()));
        connect(worker, SIGNAL(updateStarted(QString)),
                this, SLOT(doUpdateWorkStarted(QString)));
        connect(worker, SIGNAL(updateProgressReport(QString,int)),
                this, SLOT(doUpdateWorkProgressReport(QString,int)));
        connect(worker, SIGNAL(updateFinished(QString,bool)),
                this, SLOT(doUpdateWorkFinished(QString,bool)));
        workThread->start();
        m_idleTaskNumber--;
    }
}
