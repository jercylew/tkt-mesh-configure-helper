#include "addtktmeshdialog.h"
#include "ui_addtktmeshdialog.h"

AddTKTMeshDialog::AddTKTMeshDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTKTMeshDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("添加TKTMesh主机");
    setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
}

AddTKTMeshDialog::~AddTKTMeshDialog()
{
    delete ui;
}

QString AddTKTMeshDialog::getIP()
{
    return ui->lineIpAddress->text();
}

void AddTKTMeshDialog::showEvent(QShowEvent *ev)
{
    QDialog::showEvent(ev);
    ui->lineIpAddress->setFocus();
}

void AddTKTMeshDialog::on_buttonDone_clicked()
{
    accept();
}

void AddTKTMeshDialog::on_buttonCancel_clicked()
{
    reject();
}
