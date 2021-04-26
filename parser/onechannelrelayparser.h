#ifndef ONECHANNELRELAYPARSER_H
#define ONECHANNELRELAYPARSER_H

#include "executerparser.h"

class OneChannelRelayFrame;

class OneChannelRelayParser : public ExecuterParser
{
public:
    OneChannelRelayParser();
    bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer);
    void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);

protected:
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    OneChannelRelayFrame *m_oneChannelRelayFrame;
};

#endif // ONECHANNELRELAYPARSER_H
