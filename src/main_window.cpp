// main_window.cpp
#include "main_window.h"
#include "app_edit.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QProcess>
#include <QComboBox>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    comboBox = new QComboBox(this);
    mainLayout = new QVBoxLayout(centralWidget);
    launchArgumentsLabel = new QLabel("Launch arguments", this);
    commandTextEdit = new QTextEdit(this);
    launchArgumentsLabel->setFixedSize(400,30);
    commandTextEdit->setFixedSize(400, 30);
    playButton = new QPushButton("Play");
    playButton->setFixedSize(200,30);
    killButton = new QPushButton("Kill Process");
    killButton->setFixedSize(200,30);
    wineTricksButton = new QPushButton("Winetricks");
    wineTricksButton->setFixedSize(200,30);
    wineConfigButton = new QPushButton("WineConfig");
    wineConfigButton->setFixedSize(200,30);
    addGameButton = new QPushButton("Add game");
    addGameButton->setFixedSize(200,30);

    mainButtonLayout = new QHBoxLayout(centralWidget);
    mainButtonLayout->addWidget(playButton);
    mainButtonLayout->addWidget(killButton);

    wineLayout = new QHBoxLayout(centralWidget);
    wineLayout->addWidget(wineTricksButton);
    wineLayout->addWidget(wineConfigButton);

    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(launchArgumentsLabel);
    mainLayout->addWidget(commandTextEdit);
    mainLayout->addLayout(mainButtonLayout);
    mainLayout->addLayout(wineLayout);
    mainLayout->addWidget(addGameButton);

    setWindowTitle("Awu");

    connect(addGameButton, &QPushButton::clicked, this, &MainWindow::popUpWindowExec);
}

MainWindow::~MainWindow() {
}
void MainWindow::handleTestButtonClick(){
    qDebug() << "Button clicked!";
}
void MainWindow::popUpWindowExec(){
    PopupWindow popupWindow(centralWidget);
    popupWindow.exec();
}
