#ifndef WARNINGLIGHT_H
#define WARNINGLIGHT_H

#include <QString>
#include "luminaire.h"

#define WARNING_LIGHT_TYPE_TEXT   "warning_light"
#define WARNING_LIGHT_SHORT_TYPE_TEXT   "WL"

class WarningLight : public Luminaire
{
public:
    typedef enum{
        Green = 0x01,
        Yellow,
        Red
    }LightColor;

    WarningLight();

    QString typeText();
    QString displayEnglishTypeText();
    QString shortTypeText();
    static QString staticTypeText();

    void loadInUpdateExecuter(Executer *executer);

    void loadUserData(Executer *executer);

    void setPower(double power);
    double getPower();
    void setEnergy(double energy);
    double getEnergy();
    void setVoltage(double voltage);
    double getVoltage();
    void setColor(WarningLight::LightColor color);
    WarningLight::LightColor getColor();
    QString getColorText();

    QString dataText();

private:
    double power;
    double energy;
    double voltage;
    WarningLight::LightColor color;
    //m_on ?
};

#endif // WARNINGLIGHT_H
