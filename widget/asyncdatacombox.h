#ifndef ASYNCDATACOMBOX_H
#define ASYNCDATACOMBOX_H

#include <QComboBox>

class AsyncDataCombox : public QComboBox
{
    Q_OBJECT
public:
    AsyncDataCombox(QWidget *parent=0);

    void showPopup();

signals:
    void aboutToShowPopup();
};

#endif // ASYNCDATACOMBOX_H
