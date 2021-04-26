#ifndef LANGUAGEDIALOGFRAME_H
#define LANGUAGEDIALOGFRAME_H

#include <QFrame>

namespace Ui {
class LanguageDialogFrame;
}

class LanguageDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit LanguageDialogFrame(QWidget *parent = 0);
    ~LanguageDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonApply_clicked();

    void on_buttonClose_clicked();

private:
    Ui::LanguageDialogFrame *ui;

signals:
    void reject();
};

#endif // LANGUAGEDIALOGFRAME_H
