#ifndef DISHSELLINGTABLE_H
#define DISHSELLINGTABLE_H

#include "executer.h"

#define DISH_SELLING_TABLE_TYPE_TEXT    "dish_selling_table"
#define DISH_SELLING_TABLE_SHORT_TYPE_TEXT    "DST"

class DishSellingTable : public Executer
{
public:
    DishSellingTable();

    QString typeText();
    QString shortTypeText();
    static QString staticTypeText();
    QString dataText();
    void loadInUpdateExecuter(Executer *executer);
    QString displayEnglishTypeText();
//    double dataValue(QString strValueName = "");

    void setHeating(bool bHeating) { m_bIsHeating = bHeating; }
    bool isHeating() const { return m_bIsHeating; }

    void setWaterUp(bool bUp) { m_bIsWaterUp = bUp; }
    bool isWaterUp() const { return m_bIsWaterUp; }

    void setTempError(bool bError) { m_bIsTempError = bError; }
    bool isTempError() const { return m_bIsTempError; }

    void setWaterLevelError(bool bError) { m_bIsWaterLevelError = bError; }
    bool isWaterLevelError() const { return m_bIsWaterLevelError; }

    void setTemperature(int nTemp) { m_nTemperature = nTemp; }
    int getTemperature() const { return m_nTemperature; }

    void setWaterLevel(int nLevel) { m_nWaterLevel = nLevel; }
    int getWaterLevel() const { return m_nWaterLevel; }

private:
    bool    m_bIsHeating;
    bool    m_bIsWaterUp;
    bool    m_bIsTempError;
    bool    m_bIsWaterLevelError;
    int     m_nTemperature;
    int     m_nWaterLevel;
};

#endif // DISHSELLINGTABLE_H
