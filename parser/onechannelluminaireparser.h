#ifndef ONECHANNELLUMINAIREPARSER_H
#define ONECHANNELLUMINAIREPARSER_H

#include "executerparser.h"

class OneChannelLuminaireFrame;

class OneChannelLuminaireParser : public ExecuterParser
{
public:
    OneChannelLuminaireParser();
    bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer);
    void parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer);
    void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);

protected:
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    OneChannelLuminaireFrame *m_oneChannelLuminaireFrame;
};

#endif // ONECHANNELLUMINAIREPARSER_H
