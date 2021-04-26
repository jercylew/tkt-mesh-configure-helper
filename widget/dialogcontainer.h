#ifndef DIALOGCONTAINER_H
#define DIALOGCONTAINER_H

#include <QDialog>

namespace Ui {
class DialogContainer;
}

class DialogContainer : public QDialog
{
    Q_OBJECT

public:
    explicit DialogContainer(bool closable, QWidget *parent = 0);
    ~DialogContainer();

    void setContentWidget(QWidget *widget);
    void setDialogTitle(const QString &title);

    void reject();

private:
    Ui::DialogContainer *ui;
    bool m_closable;

public slots:
    void setClosable(bool closable);
};

#endif // DIALOGCONTAINER_H
