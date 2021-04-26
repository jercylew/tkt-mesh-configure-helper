#include "asyncdatacombox.h"

AsyncDataCombox::AsyncDataCombox(QWidget *parent)
    : QComboBox(parent)
{
    this->setEditable(true);
}

void AsyncDataCombox::showPopup()
{
    emit aboutToShowPopup();
    QComboBox::showPopup();
}
