#ifndef ADDTIMEPOINTDIALOG_H
#define ADDTIMEPOINTDIALOG_H

#include <QFrame>
#include "domain/timelinecontrolpolicy.h"

namespace Ui {
class AddTimePointDialogFrame;
}

class MeshModel;

class AddTimePointDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AddTimePointDialogFrame(int *minutes, QWidget *parent = 0);
    ~AddTimePointDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonOk_clicked();

    void on_buttonCancel_clicked();

private:
    Ui::AddTimePointDialogFrame *ui;
    int *m_minutes;
    int getMinutes();

signals:
    void accept();
    void reject();
};

#endif // ADDTIMEPOINTDIALOG_H
