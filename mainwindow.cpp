#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "model/tktmeshesmodel.h"
#include "model/tktmeshtableviewmodel.h"
#include "addtktmeshdialog.h"
#include "model/tktmeshmodel.h"
#include "connecthostdialog.h"
#include "domain/tktmesh.h"
#include "callback/gethostinfocallback.h"
#include "utils/asyncapirequesttask.h"
#include <QJsonObject>
#include "utils/tktmeshconnectionholder.h"
#include "utils/tktmeshapiworkercontroller.h"
#include "utils/asyncapirequesttask.h"
#include "callback/sethostinfocallback.h"
#include "domain/mesh.h"
#include "callback/getmeshbasicinfocallback.h"
#include "callback/setmeshbasicinfocallback.h"
#include "callback/refreshmeshnodescallback.h"
#include "callback/getmeshallnodescallback.h"
#include "callback/defaultcallback.h"
#include <QMessageBox>
#include "domain/bluetoothnode.h"
#include "callback/getallcomcallback.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include "dialogbuilder.h"
#include "widget/nodesupdatedialog.h"

#define APP_VERSION "1.1.0.200410"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->pageTKTMeshList);

    m_tktMeshesModel=new TKTMeshesModel(this);
    m_tktMeshTableViewModel=new TKTMeshTableViewModel(m_tktMeshesModel, this);
    connect(m_tktMeshesModel, SIGNAL(scanProgress(int,int)),
            this, SLOT(doScanProgress(int,int)));
    ui->tableTKTMesh->setModel(m_tktMeshTableViewModel);

    ui->tableTKTMesh->setEditTriggers(QTableView::NoEditTriggers);
    ui->tableTKTMesh->verticalHeader()->setVisible( true );
    ui->tableTKTMesh->horizontalHeader()->setStretchLastSection( true );
    ui->tableTKTMesh->setSelectionMode(QTableView::ExtendedSelection);
    ui->tableTKTMesh->setSelectionBehavior(QTableView::SelectRows);

    m_tktMeshModel=NULL;

    ui->labelStatus->setStyleSheet("color: #FF0000;");

    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Interactive);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Interactive);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Interactive);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(9, QHeaderView::Stretch);
    ui->tableTKTMesh->horizontalHeader()->setSectionResizeMode(10, QHeaderView::Stretch);

    ui->combDBPackageMaxSizeInKB->setCurrentIndex(ui->combDBPackageMaxSizeInKB->findText("10"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(QMessageBox::question(this, "退出软件", "确定退出本软件?")==QMessageBox::Yes)
    {
        event->accept();
    }
    else
    {
        event->ignore();
    }
}

void MainWindow::showPopupStatusText(const QString &text, bool showMovie)
{
    m_statusDisplayDialog.setStatusText(text, showMovie);
    if(m_statusDisplayDialog.isHidden())
    {
        m_statusDisplayDialog.show();
    }
}

void MainWindow::hidePopupStatusText()
{
    m_statusDisplayDialog.hide();
}

void MainWindow::setStatusText(const QString &text, const QString &color)
{
    if(text.isEmpty())
    {
        ui->labelStatus->setText("");
    }
    else
    {
        ui->labelStatus->setText(QString("[%1] %2").arg(QTime::currentTime().toString("hh:mm:ss")).arg(text));
        ui->labelStatus->setStyleSheet(QString("color:%1;").arg(color));
    }
}

void MainWindow::on_buttonAddTKTMesh_clicked()
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
    m_tktMeshesModel->scanTKTMesh(address, address, ui->spinWaitMsec->value(), false);
}

void MainWindow::on_buttonScanTKTMesh_clicked()
{
    ui->buttonScanTKTMesh->setEnabled(false);
    ui->buttonAddTKTMesh->setEnabled(false);
    ui->lineFromAddress->setEnabled(false);
    ui->lineToAddress->setEnabled(false);
    ui->labelStatusText->setText("扫描中...");
    QHostAddress address1(ui->lineFromAddress->text());
    QHostAddress address2(ui->lineToAddress->text());
    m_tktMeshesModel->scanTKTMesh(address1, address2, ui->spinWaitMsec->value(), true);
}

void MainWindow::doScanProgress(int total, int finished)
{
    if(total==finished)
    {
        ui->labelStatusText->setText(tr("扫描完成, 双击下列表格单元进行连接操作!"));
        ui->buttonScanTKTMesh->setEnabled(true);
        ui->buttonAddTKTMesh->setEnabled(true);
        ui->lineFromAddress->setEnabled(true);
        ui->lineToAddress->setEnabled(true);
    }
    else
    {
        ui->labelStatusText->setText(tr("扫描进度: %1 / %2").arg(finished).arg(total));
    }
}

void MainWindow::on_tableTKTMesh_doubleClicked(const QModelIndex &index)
{
    connectToHost();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "关于", QString("软件名称: TKTMesh配置助手\n软件版本: %1").arg(APP_VERSION));
}

void MainWindow::on_actionExit_triggered()
{
    qApp->closeAllWindows();
}

void MainWindow::doConnectHostFinished(bool ok)
{
    if(ok)
    {
        QLayout *layout=ui->pageTKTMesh->layout();
        if(layout==NULL)
        {
            layout=new QVBoxLayout;
            layout->setContentsMargins(5, 5, 5, 5);
            ui->pageTKTMesh->setLayout(layout);
        }
        m_detailsFrame=new TKTMeshDetailsFrame(this->m_tktMeshModel, this);
        connect(m_detailsFrame, SIGNAL(backToTKTMeshList()), this, SLOT(doBacktoTKTMeshList()));
        layout->addWidget(m_detailsFrame);
        ui->stackedWidget->setCurrentWidget(ui->pageTKTMesh);
    }
    else
    {
        if(m_tktMeshModel->tktMesh()->inCloudConfigMode())
        {
            setStatusText(QString("连接云配置主机\"%1\"失败!").arg(m_tktMeshModel->tktMesh()->cloudConfigAddress()));
        }
        else
        {
            setStatusText(QString("连接本地主机\"%1\"失败!").arg(m_tktMeshModel->tktMesh()->hostPeerIP()));
        }
        m_tktMeshModel->deleteLater();
        m_tktMeshModel=NULL;
    }
    hidePopupStatusText();
}

void MainWindow::doBacktoTKTMeshList()
{
    ui->stackedWidget->setCurrentWidget(ui->pageTKTMeshList);
    if(m_tktMeshModel!=NULL)
    {
        m_tktMeshModel->deleteLater();
        m_tktMeshModel=NULL;
    }
    if(m_detailsFrame!=NULL)
    {
        m_detailsFrame->deleteLater();
        m_detailsFrame=NULL;
    }
}

void MainWindow::connectToHost()
{
    if(m_tktMeshModel!=NULL)
    {
        return;
    }
    TKTMesh *tktMesh=this->m_tktMeshesModel->getByID(ui->tableTKTMesh->currentIndex().data(Qt::UserRole).toString());
    tktMesh->setDbDataPackageMaxSizeInKB(ui->combDBPackageMaxSizeInKB->currentText().toInt());
    if(tktMesh==NULL)
    {
        return;
    }
    m_tktMeshModel=new TKTMeshModel(tktMesh, this);
    connect(this->m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController(), SIGNAL(connectHostFinished(bool)),
            this, SLOT(doConnectHostFinished(bool)));
    this->m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->connectHost();

    showPopupStatusText(QString("正在连接..."), true);
}

void MainWindow::on_buttonRemoteUpgrade_clicked()
{
    NodesUpdateDialog *dialog=new NodesUpdateDialog(this);
    dialog->setWindowTitle(tr("Update Nodes"));
    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::on_buttonStartCloudConfig_clicked()
{
    if(m_tktMeshModel!=NULL)
    {
        return;
    }
    TKTMesh *tktMesh=new TKTMesh();
    tktMesh->setDbDataPackageMaxSizeInKB(ui->combDBPackageMaxSizeInKB->currentText().toInt());
    tktMesh->setInCloudConfigMode(true);
    tktMesh->setHostID(ui->lineCloudConfigTargetHostID->text().trimmed());
    tktMesh->setCloudConfigAddress(ui->lineCloudConfigAddress->text().trimmed());
    tktMesh->setCloudConfigConfigerPort(ui->lineCloudConfigPort->text().trimmed().toInt());
    m_tktMeshModel=new TKTMeshModel(tktMesh, this);
    connect(this->m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController(), SIGNAL(connectHostFinished(bool)),
            this, SLOT(doConnectHostFinished(bool)));
    this->m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->connectHost();
    if(tktMesh->inCloudConfigMode())
    {
        showPopupStatusText(QString("正在连接云端配置主机\"%1\"...").arg(m_tktMeshModel->tktMesh()->cloudConfigAddress()), true);
    }
    else
    {
        showPopupStatusText(QString("正在连接主机\"%1\"...").arg(m_tktMeshModel->tktMesh()->hostPeerIP()), true);
    }
}

void MainWindow::on_actionTemplateManagement_triggered()
{
}
