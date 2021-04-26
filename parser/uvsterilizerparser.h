#ifndef UVSTERILIZERPARSER_H
#define UVSTERILIZERPARSER_H

#include "executerparser.h"

class UVSterilizerFrame;

class UVSterilizerParser : public ExecuterParser
{
public:
    UVSterilizerParser();
    bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer);
    void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);
    void updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);
    void parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer);

protected:
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    UVSterilizerFrame *m_ptrUVSterilizerFrame;
};

#endif // UVSTERILIZERPARSER_H
