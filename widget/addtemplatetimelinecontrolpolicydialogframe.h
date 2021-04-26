#ifndef ADDTEMPLATETIMELINECONTROLITEMCOLLECTIONDIALOGFRAME_H
#define ADDTEMPLATETIMELINECONTROLITEMCOLLECTIONDIALOGFRAME_H

#include <QFrame>
#include "model/templatetimelinecontrolpolicylisteditmodel.h"

namespace Ui {
class AddTemplateTimeLineControlPolicyDialogFrame;
}

class AddTemplateTimeLineControlPolicyDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AddTemplateTimeLineControlPolicyDialogFrame(QString *name, QString *description, QWidget *parent = 0);
    ~AddTemplateTimeLineControlPolicyDialogFrame();

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

signals:
    void accept();
    void reject();

private:
    Ui::AddTemplateTimeLineControlPolicyDialogFrame *ui;
    QString *name;
    QString *description;
};

#endif // EDITTEMPLATETIMELINECONTROLITEMCOLLECTIONDIALOGFRAME_H
