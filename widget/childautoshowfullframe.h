#ifndef CHILDAUTOSHOWFULLFRAME_H
#define CHILDAUTOSHOWFULLFRAME_H

#include <QFrame>

class ChildAutoShowFullFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ChildAutoShowFullFrame(QWidget *parent = 0);

    void setChildWidget(QWidget *childWidget);

    void resizeEvent(QResizeEvent *event);

private:
    QWidget *m_childWidget;
    void childShowFull();

signals:

public slots:
};

#endif // CHILDAUTOSHOWFULLFRAME_H
