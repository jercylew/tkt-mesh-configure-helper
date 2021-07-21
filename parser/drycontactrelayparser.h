#ifndef DRYCONTACTRELAYPARSER_H
#define DRYCONTACTRELAYPARSER_H

#include "executerparser.h"

class DryContactRelayFrame;

class DryContactRelayParser : public ExecuterParser
{
public:
    DryContactRelayParser();
    bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer);
    void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);
    void updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);

protected:
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    DryContactRelayFrame *m_dryContactRelayFrame;
};


#endif // DRYCONTACTRELAYPARSER_H
