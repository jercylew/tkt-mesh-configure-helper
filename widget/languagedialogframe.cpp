#include "languagedialogframe.h"
#include "ui_languagedialogframe.h"
#include "languagemanager.h"
#include"globalsavingbox.h"

LanguageDialogFrame::LanguageDialogFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::LanguageDialogFrame)
{
    ui->setupUi(this);

    QMap<QString, QString> markToNameMap=LanguageManager::instance()->getSupportedLanguageMarkNameMap();
    QList<QString> markList=markToNameMap.keys();
    for(int i=0; i<markList.size(); i++)
    {
        QString key=markList.at(i);
        ui->combLanguages->addItem(markToNameMap.value(key), key);
    }
    int currentIndex=ui->combLanguages->findData(GlobalSavingBox::getInstance()->getValue("Global.Language"));
    if(currentIndex<0)
    {
        currentIndex=ui->combLanguages->findData("en_US");
    }
    if(currentIndex>=0)
    {
        ui->combLanguages->setCurrentIndex(currentIndex);
    }

    ui->buttonApply->setCursor(Qt::PointingHandCursor);
    ui->buttonClose->setCursor(Qt::PointingHandCursor);
}

LanguageDialogFrame::~LanguageDialogFrame()
{
    delete ui;
}

void LanguageDialogFrame::changeEvent(QEvent *ev)
{
    if(ev->type()==QEvent::LanguageChange)
    {
        ui->retranslateUi(this);
    }
}

void LanguageDialogFrame::on_buttonApply_clicked()
{
    LanguageManager::instance()->changeLanguage(ui->combLanguages->currentData().toString());
    GlobalSavingBox::getInstance()->setValue("Global.Language", ui->combLanguages->currentData().toString());
}

void LanguageDialogFrame::on_buttonClose_clicked()
{
    emit reject();
}
