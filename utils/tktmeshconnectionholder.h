#ifndef TKTMESHCONNECTIONHOLDER_H
#define TKTMESHCONNECTIONHOLDER_H

#include <QObject>
#include <QTcpSocket>
#include "tktmeshapiworkercontroller.h"

class TKTMesh;

class TKTMeshConnectionHolder : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshConnectionHolder(TKTMesh *tktMesh, QObject *parent = 0);
    ~TKTMeshConnectionHolder();

    TKTMeshApiWorkerController *apiWorkerController();

private:
    TKTMesh *m_tktMesh;
    TKTMeshApiWorkerController *m_apiWorkerController;


};

#endif // TKTMESHCONNECTIONHOLDER_H
