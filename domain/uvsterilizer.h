#ifndef UVSTERILIZER_H
#define UVSTERILIZER_H

#include "executer.h"

#define UV_STERILIZER_TYPE_TEXT         "uv_sterilizer"
#define UV_STERILIZER_SHORT_TYPE_TEXT   "UVS"

class UVSterilizer : public Executer
{
public:
    UVSterilizer();

    QString typeText();
    QString shortTypeText();
    static QString staticTypeText();

    void loadInUpdateExecuter(Executer *executer);
    bool configState() const { return m_bConfigState; }
    QString configStateString() const;
    void setConfigState(bool bState) { m_bConfigState = bState; }
    QString sterilizeStateString() const;
    bool sterilizeState() const { return m_bSterilizeState; }
    void setSterilizeState(bool bState) { m_bSterilizeState = bState; }
    quint8 wifiAddress() const { return m_nWifiAddress; }
    void setWifiAddress(quint8 nWifiAddress) { m_nWifiAddress = nWifiAddress; }
    qint8 funcCode() const { return m_nFuncCode; }
    void setFuncCode(qint8 nConde) { m_nFuncCode =  nConde; }

    QString displayEnglishTypeText();
    QString dataText();
    bool isLuminaire() { return false; }

private:
    bool m_bConfigState;        //true: Config Success, 0x00: Config Failure
    bool m_bSterilizeState;     //true: Sterilizing Completed, false: Serilizing in Process
    quint8 m_nWifiAddress;
    qint8 m_nFuncCode;
};

#endif // UVSTERILIZER_H
