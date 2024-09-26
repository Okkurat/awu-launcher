// app_edit.h
#ifndef APPE_DIT_H
#define APPE_DIT_H
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QComboBox>


class PopupWindow : public QDialog {
    Q_OBJECT

public:
    PopupWindow(QWidget *parent = nullptr,
    const QString& configFile = "");
    ~PopupWindow();

private slots:
    void doneFn(const QString& gameFile);
    void searchDatabase();
    void setPrefix();
    void setExe();
    void protonComboBoxChanged(int index);
private:
    QWidget *centralWidget;
    QVBoxLayout *layout;
    QHBoxLayout *doneCloseLayout;
    QHBoxLayout *gameNameLayout;
    QHBoxLayout *prefixLayout;
    QHBoxLayout *gameExeLayout;
    QLabel *gameLabel;
    QLabel *gameExeLabel;
    QLabel *protonLabel;
    QLabel *prefixLabel;
    QTextEdit *gameName;
    QTextEdit *prefixPath;
    QTextEdit *gameExeEdit;
    QPushButton *closeButton;
    QPushButton *doneButton;
    QPushButton *searchButton;
    QPushButton *prefixButton;
    QPushButton *gameExeButton;
    QComboBox *protonComboBox;
    QLabel *gameIdLabel;
    QTextEdit *gameIdEdit;
    QLabel *launchArgsLabel;
    QTextEdit *launchArgsEdit;
    QLabel *storeLabel;
    QComboBox *storeComboBox;
    QLabel *awuArgsLabel;
    QTextEdit *awuArgsEdit;
    void populateProtonBox();
};

#endif // APP_EDIT_H
