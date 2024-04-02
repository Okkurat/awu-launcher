// app_edit.h
#ifndef APPE_DIT_H
#define APPE_DIT_H

#include <QDialog>

class PopupWindow : public QDialog {
    Q_OBJECT

public:
    PopupWindow(QWidget *parent = nullptr);
    ~PopupWindow();
};

#endif // APP_EDIT_H
