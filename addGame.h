// addGame.h
#ifndef ADDGAME_H
#define ADDGAME_H

#include <QDialog>

class PopupWindow : public QDialog {
    Q_OBJECT

public:
    PopupWindow(QWidget *parent = nullptr);
    ~PopupWindow();
};

#endif // ADDGAME_H
