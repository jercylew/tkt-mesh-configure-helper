#ifndef DISHSELLINGTABLEPARSER_H
#define DISHSELLINGTABLEPARSER_H

#include "executerparser.h"

class DishSellingTableFrame;

class DishSellingTableParser : public ExecuterParser
{
public:
    DishSellingTableParser();

    bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer);
    void parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer);
    void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);
    void updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);

protected:
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    DishSellingTableFrame *m_dishSellingTableFrame;
};

#endif // DISHSELLINGTABLEPARSER_H
