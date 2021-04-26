#ifndef ONECHANNELLUMINAIRE_H
#define ONECHANNELLUMINAIRE_H

#include "luminaire.h"

#define ONE_CHANNEL_LUMINAIRE_TYPE_TEXT   "luminaire"
#define ONE_CHANNEL_LUMINAIRE_SHORT_TYPE_TEXT   "C1L"

class OneChannelLuminaire : public Luminaire
{
public:
    OneChannelLuminaire();

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
    QString dataText();

private:
    double power;
    double energy;
    double voltage;
};

#endif // ONECHANNELLUMINAIRE_H
