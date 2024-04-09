// main_window.h
#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QProcess>
#include <QComboBox>
#include <QTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

private slots:
    void popUpWindowExec();
    void runWineTricks();
    void runWineConfig();
    void comboBoxCurrentIndexChanged(int index);
    void killProcessFn();
    void runGameProcessFn();

private:
    QComboBox *comboBox;
    QTextEdit *commandTextEdit;
    QLabel *launchArgumentsLabel;
    QPushButton *playButton;
    QPushButton *killButton;
    QPushButton *wineTricksButton;
    QPushButton *wineConfigButton;
    QPushButton *addGameButton;
    QVBoxLayout *mainLayout;
    QHBoxLayout *mainButtonLayout;
    QHBoxLayout *wineLayout;
    QProcess *gameProcess;
    QProcess *killProcess;
    QProcess *wineTricksProcess;
    QProcess *wineConfigProcess;
    QWidget *centralWidget;
};

#endif // MAIN_WINDOW_H
