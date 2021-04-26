#ifndef TKTMESHUPDATEDISCOVERHELPER_H
#define TKTMESHUPDATEDISCOVERHELPER_H


#include <QObject>
#include <QHostAddress>

class TKTMesh;

class TKTMeshUpdateDiscoverHelper : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshUpdateDiscoverHelper(QObject *parent = 0);

    void scan(const QHostAddress &address1, const QHostAddress &address2, int waitMsec);

private:
    int scanTotal;
    int scanFinished;

signals:
    void discovered(TKTMesh *tktMesh);
    void scanProgress(int total, int finished);

public slots:
    void doAsyncScanTKTMeshTaskFinished();
};

#endif // TKTMESHUPDATEDISCOVERHELPER_H
