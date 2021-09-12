#include "dishsellingtable.h"

DishSellingTable::DishSellingTable()
{
    m_bIsHeating = false;
    m_bIsWaterUp = false;
    m_bIsTempError = false;
    m_bIsWaterLevelError = false;
    m_nTemperature = 0;
    m_nWaterLevel = 0;
}

QString DishSellingTable::typeText()
{
    return DISH_SELLING_TABLE_TYPE_TEXT;
}

QString DishSellingTable::shortTypeText()
{
    return DISH_SELLING_TABLE_SHORT_TYPE_TEXT;
}

QString DishSellingTable::staticTypeText()
{
    return DISH_SELLING_TABLE_TYPE_TEXT;
}

void DishSellingTable::loadInUpdateExecuter(Executer *executer)
{
    Executer::loadInUpdateExecuter(executer);
    if(executer->typeText()==DishSellingTable::staticTypeText())
    {
        DishSellingTable *table = static_cast<DishSellingTable*>(executer);
        if(table)
        {
            setHeating(table->isHeating());
            setWaterUp(table->isWaterUp());
            setWaterLevelError(table->isWaterLevelError());
            setTempError(table->isTempError());
            setTemperature(table->getTemperature());
            setWaterLevel(table->getWaterLevel());
        }
    }
}

QString DishSellingTable::dataText()
{
    QString strStatMsg = "";

    if (m_bIsHeating)
    {
        strStatMsg.append(QObject::tr("Heating, "));
    }
    if (m_bIsWaterUp)
    {
        strStatMsg.append(QObject::tr("Water up, "));
    }
    if (m_bIsTempError)
    {
        strStatMsg.append(QObject::tr("Temp Error, "));
    }
    if (m_bIsWaterLevelError)
    {
        strStatMsg.append(QObject::tr("Water Level Error"));
    }
    return QObject::tr("TEMP: %1, WATER LVL: %2, %3")
            .arg(m_nTemperature)
            .arg(m_nWaterLevel)
            .arg(strStatMsg);
}

QString DishSellingTable::displayEnglishTypeText()
{
    return "Dish Selling Table";
}
