#ifndef INPUTDIALOGFRAME_H
#define INPUTDIALOGFRAME_H

#include <QFrame>

namespace Ui {
class InputDialogFrame;
}

class InputDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit InputDialogFrame(QString *text, const QString &inputTitle, QWidget *parent = 0);
    ~InputDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDialogDone_clicked();

    void on_buttonDialogCancel_clicked();

private:
    Ui::InputDialogFrame *ui;

    QString *text;

signals:
    void accept();
    void reject();
};

#endif // INPUTDIALOGFRAME_H
