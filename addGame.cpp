// addGame.cpp
#include "addGame.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

PopupWindow::PopupWindow(QWidget *parent) : QDialog(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QLabel *label = new QLabel("This is a popup window", this);
    layout->addWidget(label);

    QPushButton *closeButton = new QPushButton("Close", this);
    layout->addWidget(closeButton);

    connect(closeButton, &QPushButton::clicked, this, &PopupWindow::close);
}

PopupWindow::~PopupWindow() {
}
