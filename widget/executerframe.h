#ifndef EXECUTERFRAME_H
#define EXECUTERFRAME_H

#include <QFrame>

class QLabel;

namespace Ui {
class ExecuterFrame;
}

class ExecuterFrame : public QFrame
{
    Q_OBJECT

public:
    explicit ExecuterFrame(QWidget *parent = 0);
    ~ExecuterFrame();

    QLabel *getIconLabel();
    QLabel *getTextLabel();

private:
    Ui::ExecuterFrame *ui;
};

#endif // EXECUTERFRAME_H
