#ifndef EXECUTERPARSER_H
#define EXECUTERPARSER_H

#include "parser.h"
#include <QSqlQuery>
#include <QPixmap>

class Executer;
class MeshModel;
class ExecuterFrame;

class ExecuterParser : public Parser
{
public:
    ExecuterParser();

    virtual void parseDBDataToExecuter(QSqlQuery *query, Executer *executer);
    virtual bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer);
    virtual void parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer);
    virtual void updateReferredExecuterDataAndNotify(MeshModel *meshModel, Executer *executer, bool dbReason=false);

    virtual void updateExecuterFromDB(MeshModel *meshModel, Executer *dbExecuter, Executer *oldExecuter);
    virtual void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);
    virtual void updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);

    virtual void parseExecuterDataToExecuter(const QString &executerData, Executer *executer);

    virtual QString getExecuterTooltipInExecuterListView(Executer *executer);
    QPixmap getSurfaceWithTouchInfo(MeshModel *meshModel, Executer *executer);

protected:
    ExecuterFrame *getExecuterFrame();
    virtual QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    ExecuterFrame *m_executerFrame;
};

#endif // EXECUTERPARSER_H
