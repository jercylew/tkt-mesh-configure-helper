#ifndef GETDATEDIALOGFRAME_H
#define GETDATEDIALOGFRAME_H

#include <QFrame>

namespace Ui {
class GetDateDialogFrame;
}

class GetDateDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit GetDateDialogFrame(QDate *date, QWidget *parent = 0);
    ~GetDateDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDialogDone_clicked();

    void on_buttonDialogCancel_clicked();

private:
    Ui::GetDateDialogFrame *ui;
    QDate *date;

signals:
    void accept();
    void reject();
};

#endif // GETDATEDIALOGFRAME_H
