#include "connecthostdialog.h"
#include "ui_connecthostdialog.h"
#include "model/tktmeshmodel.h"
#include "utils/tktmeshconnectionholder.h"
#include <QTimer>

ConnectHostDialog::ConnectHostDialog(TKTMeshModel *tktMeshModel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectHostDialog)
{
    ui->setupUi(this);
    this->m_tktMeshModel=tktMeshModel;
    connect(this->m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController(), SIGNAL(connectHostFinished(bool)),
            this, SLOT(doConnectHostFinished(bool)));
    QTimer::singleShot(100, this, SLOT(doTimeout()));
    setWindowFlags(Qt::Dialog);
}

ConnectHostDialog::~ConnectHostDialog()
{
    delete ui;
}

void ConnectHostDialog::doConnectHostFinished(bool ok)
{
    if(ok)
    {
        accept();
    }
    else
    {
        reject();
    }
}

void ConnectHostDialog::doTimeout()
{
    this->m_tktMeshModel->tktMeshConnectionHolder()->apiWorkerController()->connectHost();
}
