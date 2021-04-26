#ifndef ABOUTDIALOGFRAME_H
#define ABOUTDIALOGFRAME_H

#include <QFrame>

namespace Ui {
class AboutDialogFrame;
}

class AboutDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit AboutDialogFrame(QWidget *parent = 0);
    ~AboutDialogFrame();

    void setText(const QString &text);

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDialogOk_clicked();

private:
    Ui::AboutDialogFrame *ui;

signals:
    void accept();
};

#endif // ABOUTDIALOGFRAME_H
