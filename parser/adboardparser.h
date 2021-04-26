#ifndef ADBOARDPARSER_H
#define ADBOARDPARSER_H

#include "executerparser.h"

class ADBoardFrame;

class ADBoardParser : public ExecuterParser
{
public:
    ADBoardParser();

    bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer);
    void parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer);
    QString getExecuterTooltipInExecuterListView(Executer *executer);
    void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);
    void updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);

protected:
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    ADBoardFrame *m_adBoardFrame;
};

#endif // ADBOARDPARSER_H
