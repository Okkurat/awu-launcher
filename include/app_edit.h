// app_edit.h
#ifndef APPE_DIT_H
#define APPE_DIT_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

class PopupWindow : public QDialog {
    Q_OBJECT

public:
    PopupWindow(QWidget *parent = nullptr);
    ~PopupWindow();

private:
    QVBoxLayout *layout;
    QLabel *label;
    QPushButton *closeButton;
};

#endif // APP_EDIT_H
