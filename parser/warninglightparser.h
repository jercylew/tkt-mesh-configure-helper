#ifndef WARNINGLIGHTPARSER_H
#define WARNINGLIGHTPARSER_H

#include "executerparser.h"

class WarningLightFrame;

class WarningLightParser : public ExecuterParser
{
public:
    WarningLightParser();
    bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer);
    void parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer);
    void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);

protected:
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    WarningLightFrame *m_warningLightFrame;
};

#endif // WARNINGLIGHTPARSER_H
