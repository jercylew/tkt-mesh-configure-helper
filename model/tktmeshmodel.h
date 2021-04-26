#ifndef TKTMESHMODEL_H
#define TKTMESHMODEL_H

#include <QObject>

class TKTMesh;
class TKTMeshConnectionHolder;

class TKTMeshModel : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshModel(TKTMesh *tktMesh, QObject *parent = 0);
    ~TKTMeshModel();
    TKTMeshConnectionHolder *tktMeshConnectionHolder();
    TKTMesh *tktMesh();

private:
    TKTMesh *m_tktMesh;
    TKTMeshConnectionHolder *m_tktMeshConnectionHolder;

signals:

public slots:
};

#endif // TKTMESHMODEL_H
