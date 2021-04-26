#ifndef ADBOARDCONTROLDIALOGFRAME_H
#define ADBOARDCONTROLDIALOGFRAME_H

#include <QFrame>
#include <QVariant>

class MeshModel;

namespace Ui {
class ADBoardControlDialogFrame;
}

class ADBoardControlDialogFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ADBoardControlDialogFrame(MeshModel *meshModel, const QVariant &id, bool isGroup, QWidget *parent = 0);
    ~ADBoardControlDialogFrame();

private slots:
    void on_buttonDialogBottomClose_clicked();
    void on_buttonSetADBoardDisplayMode_clicked();

private:
    Ui::ADBoardControlDialogFrame *ui;
    QVariant m_id;
    MeshModel *m_meshModel;
    bool m_isGroup;

signals:
    void accept();
};

#endif // ADBOARDCONTROLDIALOGFRAME_H
