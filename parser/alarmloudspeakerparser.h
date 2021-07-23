#ifndef ALARMLOUDSPEAKERPARSER_H
#define ALARMLOUDSPEAKERPARSER_H

#include "executerparser.h"

class AlarmLoudspeakerFrame;

class AlarmLoudspeakerParser : public ExecuterParser
{
public:
    AlarmLoudspeakerParser();
    bool parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer);
    void updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);
    void updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter);

protected:
    QPixmap buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer);

private:
    AlarmLoudspeakerFrame *m_alarmLoudspeakerFrame;
};

#endif // ALARMLOUDSPEAKERPARSER_H
