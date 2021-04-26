#ifndef RENAMEDIALOGFRAME_H
#define RENAMEDIALOGFRAME_H

#include <QFrame>

namespace Ui {
class RenameDialogFrame;
}

class MeshModel;

class RenameDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit RenameDialogFrame(MeshModel *meshModel, const QString &type, const QList<QString> &idList, QWidget *parent = 0);
    ~RenameDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDone_clicked();

    void on_buttonCancel_clicked();

    void on_combNameTemplate_currentTextChanged(const QString &arg1);

    QString findSharp(const QString &text);

    QString findNumber(const QString &text);

    void on_radioFixedNumber_clicked(bool checked);

    void on_radioAutoDetected_clicked(bool checked);

    void on_spinFixedNumber_valueChanged(int arg1);

    void on_spinAutoDetected_valueChanged(int arg1);

private:
    Ui::RenameDialogFrame *ui;
    QString type;
    QList<QString> idList;
    MeshModel *m_meshModel;

signals:
    void accept();
    void reject();


};

#endif // RENAMEDIALOGFRAME_H
