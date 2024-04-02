// appedit.h
#ifndef APPEDIT_H
#define APPEDIT_H

#include <QDialog>

class PopupWindow : public QDialog {
    Q_OBJECT

public:
    PopupWindow(QWidget *parent = nullptr);
    ~PopupWindow();
};

#endif // APPEDIT_H
