#include "dishsellingtableparser.h"
#include "domain/dishsellingtable.h"
#include "model/meshmodel.h"
#include "model/nodegroupsmodel.h"
#include "widget/dishsellingtableframe.h"
#include <QPainter>

DishSellingTableParser::DishSellingTableParser()
{
    m_dishSellingTableFrame = nullptr;
}

bool DishSellingTableParser::parseOnlineStatus4BytesDataFrameToExecuter(const QByteArray &dataFrame4BytesSection, Executer *executer)
{
    return ExecuterParser::parseOnlineStatus4BytesDataFrameToExecuter(dataFrame4BytesSection, executer);
}

void DishSellingTableParser::parseUserDataFrameToExecuter(const QByteArray &dataFrame, Executer *executer)
{
    ExecuterParser::parseUserDataFrameToExecuter(dataFrame, executer);
    DishSellingTable *table = static_cast<DishSellingTable*>(executer);

    if(table)
    {
        table->setTemperature((quint16)(((quint8)dataFrame.at(12))*256+(quint8)dataFrame.at(13)));
        table->setWaterLevel((quint16)(((quint8)dataFrame.at(14))*256+(quint8)dataFrame.at(15)));
        table->setHeating(dataFrame.at(16) & 0x01);
        table->setWaterUp(dataFrame.at(16) & 0x02);
        table->setTempError(dataFrame.at(16) & 0x03);
        table->setWaterLevelError(dataFrame.at(16) & 0x04);
    }
}

void DishSellingTableParser::updateExecuterFromOnlineStatus(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromOnlineStatus(meshModel, newExecuter, oldExecuter);
    DishSellingTable *newOne=static_cast<DishSellingTable*>(newExecuter);
    DishSellingTable *oldOne=static_cast<DishSellingTable*>(oldExecuter);
    if(newOne!=NULL && oldOne!=NULL)
    {
        oldOne->setHeating(oldOne->isHeating());
        oldOne->setWaterUp(oldOne->isWaterUp());
        oldOne->setWaterLevelError(oldOne->isWaterLevelError());
        oldOne->setTempError(oldOne->isTempError());
        oldOne->setTemperature(oldOne->getTemperature());
        oldOne->setWaterLevel(oldOne->getWaterLevel());
    }
}

void DishSellingTableParser::updateExecuterFromUserDataNotify(MeshModel *meshModel, Executer *newExecuter, Executer *oldExecuter)
{
    ExecuterParser::updateExecuterFromUserDataNotify(meshModel, newExecuter, oldExecuter);
    DishSellingTable *newOne=static_cast<DishSellingTable*>(newExecuter);
    DishSellingTable *oldOne=static_cast<DishSellingTable*>(oldExecuter);
    if(newOne!=NULL && oldOne!=NULL)
    {
        oldOne->setHeating(oldOne->isHeating());
        oldOne->setWaterUp(oldOne->isWaterUp());
        oldOne->setWaterLevelError(oldOne->isWaterLevelError());
        oldOne->setTempError(oldOne->isTempError());
        oldOne->setTemperature(oldOne->getTemperature());
        oldOne->setWaterLevel(oldOne->getWaterLevel());
    }
}

QPixmap DishSellingTableParser::buildSurfaceWithNoTouchInfo(MeshModel *meshModel, Executer *executer)
{
    DishSellingTable *one=static_cast<DishSellingTable*>(executer);
    if(one==NULL)
    {
        return ExecuterParser::getSurfaceWithTouchInfo(meshModel, executer);
    }
    if(m_dishSellingTableFrame == nullptr)
    {
        m_dishSellingTableFrame = new DishSellingTableFrame;
    }
    m_dishSellingTableFrame->setGroupColors(meshModel->nodeGroupsModel()->getColorList(executer->getGroupList()));
    m_dishSellingTableFrame->setName(one->getName().isEmpty()?one->executerId():one->getName());
    if(one->isOnline())
    {
        m_dishSellingTableFrame->getIconLabel()->setProperty("status", "dish_selling_table_online");
    }
    else
    {
        m_dishSellingTableFrame->getIconLabel()->setProperty("status", "dish_selling_table_offline");
    }
    m_dishSellingTableFrame->resetStyle();

    QPixmap basePixmap = m_dishSellingTableFrame->grab();

    //Draw status icon to the base pixmap
     QPainter painter(&basePixmap);
    if (one->isHeating())
    {
        painter.drawImage(10, 0, QImage(":/images/dst_heating.png"));
    }
    if (one->isWaterUp())
    {
        painter.drawImage((basePixmap.width()/2)-10, 0, QImage(":/images/dst_water_up.png"));
    }
    if (one->isTempError())
    {
        painter.drawImage(10, basePixmap.height()-30, QImage(":/images/dst_temp_error.png"));
    }
    if (one->isWaterLevelError())
    {
        painter.drawImage(basePixmap.width()-30, basePixmap.height()-30,
                          QImage(":/images/dst_water_level_error.png"));
    }
    return basePixmap;
}
