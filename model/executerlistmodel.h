#ifndef EXECUTERLISTMODEL_H
#define EXECUTERLISTMODEL_H

#include <QObject>
#include <QList>
#include "utils/meshruntimeparameterbox.h"

class Executer;
class ExecutersModel;
class MeshSavingBox;
class SensorsModel;

class ExecuterListModel : public QObject
{
    Q_OBJECT
public:
    typedef enum{
        ShowAll,
        ShowOnlyOnline
    }ShowMode;

    explicit ExecuterListModel(ExecutersModel *executersModel, SensorsModel *sensorsModel, MeshSavingBox *savingBox, QObject *parent = 0);

    void init();

    MeshRuntimeParameterBox::ExecuterOrderMethod orderMethod();
    void setOrderMethod(MeshRuntimeParameterBox::ExecuterOrderMethod orderMethod);

    bool onlyShowOnlineNodes();
    void setOnlyShowOnlineNodes(bool onlyShowOnline);
    bool onlyShowAvailableNodes();
    void setOnlyShowAvailableNodes(bool onlyShowAvailable);

    quint8 selectedGroupId();
    void setSelectedGroupId(quint8 selectedGroupId);

    int count();
    Executer *at(int index);

    double scale();
    void setScale(double scale);

    void scaleUp();
    void scaleDown();

    void refreshList();

public slots:
    void doExecutersModelDataChanged();
    void doExecutersModelModelChanged();

signals:
    void dataChanged();
    void modelChanged();

private:
    QList<Executer *> m_executerList;
    ExecutersModel *m_executersModel;
    SensorsModel *m_sensorsModel;
    MeshSavingBox *m_savingBox;

public slots:
};

#endif // LUMINAIRELISTMODEL_H
