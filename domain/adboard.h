#ifndef ADBOARD_H
#define ADBOARD_H

#include "luminaire.h"

#define ADBOARD_TYPE_TEXT   "adboard"
#define ADBOARD_SHORT_TYPE_TEXT   "ADB"

class ADBoard : public Luminaire
{
public:
    ADBoard();

    QString typeText();
    QString shortTypeText();
    static QString staticTypeText();

    QString dataText();
    QString displayEnglishTypeText();

    int displayMode() const;
    void setDisplayMode(int displayMode);

    bool hasSpecialControl();

private:
    int m_displayMode;

};

#endif // ADBOARD_H
