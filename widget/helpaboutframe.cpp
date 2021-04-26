#include "helpaboutframe.h"
#include "ui_helpaboutframe.h"

HelpAboutFrame::HelpAboutFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HelpAboutFrame)
{
    ui->setupUi(this);
}

HelpAboutFrame::~HelpAboutFrame()
{
    delete ui;
}

void HelpAboutFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}
