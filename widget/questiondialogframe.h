#ifndef QUESTIONDIALOGFRAME_H
#define QUESTIONDIALOGFRAME_H

#include <QFrame>

namespace Ui {
class QuestionDialogFrame;
}

class QuestionDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit QuestionDialogFrame(QWidget *parent = 0);
    ~QuestionDialogFrame();

    void setQuestion(const QString &question);

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDialogOk_clicked();

    void on_buttonDialogCancel_clicked();

private:
    Ui::QuestionDialogFrame *ui;

signals:
    void accept();
    void reject();
};

#endif // QUESTIONDIALOGFRAME_H
