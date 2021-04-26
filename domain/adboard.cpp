#include "adboard.h"

ADBoard::ADBoard()
{
    m_displayMode=0;
}

QString ADBoard::typeText()
{
    return ADBOARD_TYPE_TEXT;
}

QString ADBoard::shortTypeText()
{
    return ADBOARD_SHORT_TYPE_TEXT;
}

QString ADBoard::staticTypeText()
{
    return ADBOARD_TYPE_TEXT;
}

QString ADBoard::dataText()
{
    return QObject::tr("DISPLAY MODE: %1").arg(m_displayMode);
}

QString ADBoard::displayEnglishTypeText()
{
    return "ADBoard";
}

int ADBoard::displayMode() const
{
    return m_displayMode;
}

void ADBoard::setDisplayMode(int displayMode)
{
    m_displayMode = displayMode;
}

bool ADBoard::hasSpecialControl()
{
    return true;
}


