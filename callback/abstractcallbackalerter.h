#ifndef ABSTRACTCALLBACKALERTER_H
#define ABSTRACTCALLBACKALERTER_H

#include <QString>

class AbstractCallbackAlerter
{
public:
    AbstractCallbackAlerter();

    virtual void showPopupStatusText(const QString &text, bool showMovie);
    virtual void hidePopupStatusText();
    virtual void setStatusText(const QString &text, const QString &color=QString("#000000"));
};

#endif // ABSTRACTCALLBACKALERTER_H
