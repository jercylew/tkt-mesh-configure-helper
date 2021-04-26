#ifndef TKTMESHDISCOVERHELPER_H
#define TKTMESHDISCOVERHELPER_H

#include <QObject>
#include <QHostAddress>

class TKTMesh;

class TKTMeshDiscoverHelper : public QObject
{
    Q_OBJECT
public:
    explicit TKTMeshDiscoverHelper(QObject *parent = 0);

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

#endif // TKTMESHDISCOVERHELPER_H
