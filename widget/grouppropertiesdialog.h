#ifndef GROUPPROPERTIESDIALOG_H
#define GROUPPROPERTIESDIALOG_H

#include <QDialog>

namespace Ui {
class GroupPropertiesDialog;
}

class GroupPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GroupPropertiesDialog(QWidget *parent = 0);
    ~GroupPropertiesDialog();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDialogBottomClose_clicked();

    void on_buttonDialogClose_clicked();

private:
    Ui::GroupPropertiesDialog *ui;
};

#endif // GROUPPROPERTIESDIALOG_H
