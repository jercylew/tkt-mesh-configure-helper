#include "uvsterilizer.h"

UVSterilizer::UVSterilizer()
{

}

QString UVSterilizer::typeText()
{
    return UV_STERILIZER_TYPE_TEXT;
}

QString UVSterilizer::shortTypeText()
{
    return UV_STERILIZER_SHORT_TYPE_TEXT;
}

QString UVSterilizer::staticTypeText()
{
    return UV_STERILIZER_TYPE_TEXT;
}

void UVSterilizer::loadInUpdateExecuter(Executer *executer)
{
    Executer::loadInUpdateExecuter(executer);
    if(executer->typeText() == UVSterilizer::staticTypeText())
    {
        UVSterilizer * uvs = static_cast<UVSterilizer*>(executer);
        if(uvs != nullptr)
        {
            setConfigState(uvs->configState());
            setSterilizeState(uvs->sterilizeState());
            setWifiAddress(uvs->wifiAddress());
            setFuncCode(uvs->funcCode());
        }
    }
}

QString UVSterilizer::configStateString() const
{
    return m_bConfigState ? "Succeed" : "Failed";
}

QString UVSterilizer::sterilizeStateString() const
{
    return m_bSterilizeState ? "Completed" : "In process";
}

QString UVSterilizer::displayEnglishTypeText()
{
    return "UV Sterilizer";
}

QString UVSterilizer::dataText()
{
    QString strDataText = QObject::tr("Wifi Address: %1, ").arg(m_nWifiAddress);
    if (m_nFuncCode == 0x02)
    {

        strDataText += QObject::tr("Appointed ") + configStateString();
    }
    else if (m_nFuncCode == 0x03)
    {
        strDataText += QObject::tr("Sterilizing ") + sterilizeStateString();
    }
    else
    {
        qWarning() << "UVSterilizer::dataText" << "Unknown function code:"
                   << m_nFuncCode;
    }
    return strDataText;
}

