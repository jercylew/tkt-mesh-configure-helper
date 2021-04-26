#ifndef MYPROXYSTYLE_H
#define MYPROXYSTYLE_H

#include <QProxyStyle>

class MyProxyStyle : public QProxyStyle
{
public:
    MyProxyStyle();

protected:
    virtual void drawPrimitive(PrimitiveElement element, const QStyleOption * option,
                                   QPainter * painter, const QWidget * widget = 0) const;
};

#endif // MYPROXYSTYLE_H
