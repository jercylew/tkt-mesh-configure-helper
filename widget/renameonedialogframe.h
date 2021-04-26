#ifndef RENAMEONEDIALOGFRAME_H
#define RENAMEONEDIALOGFRAME_H

#include <QFrame>

namespace Ui {
class RenameOneDialogFrame;
}

class MeshModel;

class RenameOneDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit RenameOneDialogFrame(MeshModel *meshModel, QString id, const QString &type, QWidget *parent = 0);
    ~RenameOneDialogFrame();

protected:
    void changeEvent(QEvent *ev);

private slots:
    void on_buttonDialogDone_clicked();

    void on_buttonDialogCancel_clicked();

    void on_lineNewName_returnPressed();

private:
    Ui::RenameOneDialogFrame *ui;
    QString id;
    QString type;
    MeshModel *m_meshModel;

signals:
    void accept();
    void reject();
};

#endif // RENAMEONEDIALOGFRAME_H
