// app_edit.cpp
#include "app_edit.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

PopupWindow::PopupWindow(QWidget *parent) : QDialog(parent) {
    layout = new QVBoxLayout(this);
    label = new QLabel("This is a popup test", this);
    layout->addWidget(label);

    closeButton = new QPushButton("Close", this);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &PopupWindow::close);
}

PopupWindow::~PopupWindow() {
}
