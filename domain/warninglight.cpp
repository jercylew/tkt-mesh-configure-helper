#include "warninglight.h"

WarningLight::WarningLight()
{
    setPower(0);
    setEnergy(0);
    setVoltage(0);
}

QString WarningLight::typeText()
{
    return WARNING_LIGHT_TYPE_TEXT;
}

QString WarningLight::displayEnglishTypeText()
{
    return "Warning Light";
}

QString WarningLight::shortTypeText()
{
    return WARNING_LIGHT_SHORT_TYPE_TEXT;
}

QString WarningLight::staticTypeText()
{
    return WARNING_LIGHT_TYPE_TEXT;
}

void WarningLight::loadInUpdateExecuter(Executer *executer)
{
    Luminaire::loadInUpdateExecuter(executer);
    if(executer->typeText()==WarningLight::staticTypeText())
    {
        WarningLight *one=static_cast<WarningLight*>(executer);
        if(one)
        {
            this->setBrightness(one->getBrightness());
        }
    }
}

void WarningLight::loadUserData(Executer *executer)
{
    WarningLight *one=static_cast<WarningLight*>(executer);
    if(one==NULL)
    {
        return;
    }
    this->setEnergy(one->getEnergy());
    this->setPower(one->getPower());
    this->setVoltage(one->getVoltage());
}


void WarningLight::setPower(double power)
{
    this->power=power;
}

double WarningLight::getPower()
{
    return this->power;
}

void WarningLight::setEnergy(double energy)
{
    this->energy=energy;
}

double WarningLight::getEnergy()
{
    return this->energy;
}

void WarningLight::setVoltage(double voltage)
{
    this->voltage=voltage;
}

double WarningLight::getVoltage()
{
    return this->voltage;
}

void WarningLight::setColor(WarningLight::LightColor color)
{
    this->color=color;
}

WarningLight::LightColor WarningLight::getColor()
{
    return this->color;
}

QString WarningLight::dataText()
{
    QString colorText = getColorText();
    QString statusText = isOnline() ? QObject::tr("Online") : QObject::tr("Offline");
    return QObject::tr("STATUS: %1, BRIGHTNESS: %2, COLOR: %3").arg(statusText).arg(this->getBrightness()).arg(colorText);
}

QString WarningLight::getColorText()
{
    QString colorTxt = "NA";
    switch (this->color) {
    case Green:
        colorTxt = "Green";
        break;
    case Yellow:
        colorTxt = "Yellow";
        break;
    case Red:
        colorTxt = "Red";
        break;
    default:
        break;
    }

    return colorTxt;
}
