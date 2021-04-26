#ifndef SAVEMESHDBDATADIALOGFRAME_H
#define SAVEMESHDBDATADIALOGFRAME_H

#include <QFrame>
#include "utils/asyncapirequesttask.h"
#include "callback/abstractcallbackalerter.h"
#include "callback/abstractcallbackinvoker.h"

class PrepareMeshModel;

namespace Ui {
class SaveMeshDBDataDialogFrame;
}

class SaveMeshDBDataDialogFrame : public QFrame, public AbstractCallbackAlerter, public AbstractCallbackInvoker
{
    Q_OBJECT

public:
    explicit SaveMeshDBDataDialogFrame(PrepareMeshModel *prepareMeshModel, QWidget *parent = 0);
    ~SaveMeshDBDataDialogFrame();

    void setStatusText(const QString &text, const QString &color);

private slots:
    void on_buttonClose_clicked();
    void doSetMeshDBDataFinished(bool ok);

private:
    Ui::SaveMeshDBDataDialogFrame *ui;
    PrepareMeshModel *m_prepareMeshModel;

    bool m_ok;

    QString dbFileMD5;
    quint64 dbFileTotal;
    quint64 currentOffset;

    void setMeshDBDataOnece();
    QByteArray dbData;

    void finished(bool ok, const QString &error=QString());

signals:
    void accept();
    void reject();
};

#endif // SAVEMESHDBDATADIALOGFRAME_H
