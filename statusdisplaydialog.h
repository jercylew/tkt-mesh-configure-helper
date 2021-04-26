#ifndef STATUSDISPLAYDIALOG_H
#define STATUSDISPLAYDIALOG_H

#include <QDialog>
#include <QMovie>

namespace Ui {
class StatusDisplayDialog;
}

class StatusDisplayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatusDisplayDialog(QWidget *parent = 0);
    ~StatusDisplayDialog();

    void setStatusText(const QString &text, bool showAnimate=false);
    void hideEvent(QHideEvent *event);

private:
    Ui::StatusDisplayDialog *ui;
    QMovie m_loadingMovie;
};

#endif // STATUSDISPLAYDIALOG_H
