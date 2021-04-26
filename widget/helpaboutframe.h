#ifndef HELPABOUTFRAME_H
#define HELPABOUTFRAME_H

#include <QFrame>

namespace Ui {
class HelpAboutFrame;
}

class HelpAboutFrame : public QFrame
{
    Q_OBJECT

public:
    explicit HelpAboutFrame(QWidget *parent = 0);
    ~HelpAboutFrame();

protected:
    void changeEvent(QEvent *ev);

private:
    Ui::HelpAboutFrame *ui;
};

#endif // HELPABOUTFRAME_H
