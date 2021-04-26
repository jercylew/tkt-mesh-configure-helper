#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "callback/abstractcallbackinvoker.h"
#include "statusdisplaydialog.h"
#include "widget/tktmeshdetailsframe.h"

class TKTMeshesModel;
class TKTMeshTableViewModel;
class TKTMeshModel;
class Mesh;
class MeshNodesModel;
class BluetoothNode;
class AbstractCallbackAlerter;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public AbstractCallbackAlerter
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void closeEvent(QCloseEvent *event);

    void showPopupStatusText(const QString &text, bool showMovie);

    void hidePopupStatusText();

    void setStatusText(const QString &text, const QString &color=QString("#000000"));

private slots:
    void on_buttonAddTKTMesh_clicked();

    void on_buttonScanTKTMesh_clicked();

    void doScanProgress(int total, int finished);

    void on_tableTKTMesh_doubleClicked(const QModelIndex &index);

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

    void on_actionTemplateManagement_triggered();

    void doConnectHostFinished(bool ok);

    void doBacktoTKTMeshList();

    void on_buttonRemoteUpgrade_clicked();

    void on_buttonStartCloudConfig_clicked();

private:
    Ui::MainWindow *ui;
    TKTMeshesModel *m_tktMeshesModel;
    TKTMeshTableViewModel *m_tktMeshTableViewModel;
    TKTMeshModel *m_tktMeshModel;
    MeshNodesModel *m_meshNodesModel;

    StatusDisplayDialog m_statusDisplayDialog;
    void connectToHost();

    TKTMeshDetailsFrame *m_detailsFrame;
};

#endif // MAINWINDOW_H
