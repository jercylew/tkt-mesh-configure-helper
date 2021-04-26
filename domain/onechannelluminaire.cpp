#include "onechannelluminaire.h"

OneChannelLuminaire::OneChannelLuminaire()
{
    setPower(0);
    setEnergy(0);
    setVoltage(0);
}

QString OneChannelLuminaire::typeText()
{
    return ONE_CHANNEL_LUMINAIRE_TYPE_TEXT;
}

QString OneChannelLuminaire::displayEnglishTypeText()
{
    return "One-Channel Luminaire";
}

QString OneChannelLuminaire::shortTypeText()
{
    return ONE_CHANNEL_LUMINAIRE_SHORT_TYPE_TEXT;
}

QString OneChannelLuminaire::staticTypeText()
{
    return ONE_CHANNEL_LUMINAIRE_TYPE_TEXT;
}

void OneChannelLuminaire::loadInUpdateExecuter(Executer *executer)
{
    Luminaire::loadInUpdateExecuter(executer);
    if(executer->typeText()==OneChannelLuminaire::staticTypeText())
    {
        OneChannelLuminaire *one=static_cast<OneChannelLuminaire*>(executer);
        if(one)
        {
            this->setBrightness(one->getBrightness());
        }
    }
}

void OneChannelLuminaire::loadUserData(Executer *executer)
{
    OneChannelLuminaire *one=static_cast<OneChannelLuminaire*>(executer);
    if(one==NULL)
    {
        return;
    }
    this->setEnergy(one->getEnergy());
    this->setPower(one->getPower());
    this->setVoltage(one->getVoltage());
}


void OneChannelLuminaire::setPower(double power)
{
    this->power=power;
}

double OneChannelLuminaire::getPower()
{
    return this->power;
}

void OneChannelLuminaire::setEnergy(double energy)
{
    this->energy=energy;
}

double OneChannelLuminaire::getEnergy()
{
    return this->energy;
}

void OneChannelLuminaire::setVoltage(double voltage)
{
    this->voltage=voltage;
}

double OneChannelLuminaire::getVoltage()
{
    return this->voltage;
}

QString OneChannelLuminaire::dataText()
{
    return QObject::tr("BRIGHTNESS: %1").arg(this->getBrightness());
}

