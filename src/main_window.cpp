// main_window.cpp
#include "main_window.h"
#include "app_edit.h"
#include "utils.h"
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
#include <QDir>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    gameProcess = new QProcess(this);
    killProcess = new QProcess(this);
    wineTricksProcess = new QProcess(this);
    wineConfigProcess = new QProcess(this);

    // Creating widgets
    comboBox = new QComboBox(this);
    launchArgumentsLabel = new QLabel("Launch arguments", this);
    commandTextEdit = new QTextEdit(this);
    playButton = new QPushButton("Play");
    killButton = new QPushButton("Kill Process");
    wineTricksButton = new QPushButton("Winetricks");
    wineConfigButton = new QPushButton("WineConfig");
    addGameButton = new QPushButton("Add game");

    // Setting fixed sizes for certain widgets
    launchArgumentsLabel->setFixedSize(400,30);
    commandTextEdit->setFixedSize(400, 30);
    playButton->setFixedSize(200,30);
    wineTricksButton->setFixedSize(200,30);
    killButton->setFixedSize(200,30);
    wineConfigButton->setFixedSize(200,30);
    addGameButton->setFixedSize(200,30);

    mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addWidget(comboBox);
    mainLayout->addWidget(launchArgumentsLabel);
    mainLayout->addWidget(commandTextEdit);

    // Create a horizontal layout for wine buttons
    wineLayout = new QHBoxLayout;
    wineLayout->addWidget(wineTricksButton);
    wineLayout->addWidget(wineConfigButton);


    // Create a horizontal layout for play and kill buttons
    mainButtonLayout = new QHBoxLayout;
    mainButtonLayout->addWidget(playButton);
    mainButtonLayout->addWidget(killButton);

    mainLayout->addLayout(mainButtonLayout);
    mainLayout->addLayout(wineLayout);

    mainLayout->addWidget(addGameButton);

    populateComboBox(*comboBox);
    commandTextEdit->clear();

    setWindowTitle("Awu");

    connect(addGameButton, &QPushButton::clicked, this, &MainWindow::popUpWindowExec);
    connect(wineTricksButton, &QPushButton::clicked, this, &MainWindow::runWineTricks);
    connect(wineConfigButton, &QPushButton::clicked, this, &MainWindow::runWineConfig);
    connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::comboBoxCurrentIndexChanged);
    connect(killButton, &QPushButton::clicked, this, &MainWindow::killProcessFn);
    connect(playButton, &QPushButton::clicked, this, &MainWindow::runGameProcessFn);
}

MainWindow::~MainWindow() {
    delete gameProcess;
    delete killProcess;
    delete wineTricksProcess;
    delete wineConfigProcess;
}

void MainWindow::runWineTricks(){
    QString selectedValue = getGameFile(*comboBox);
    qDebug() << selectedValue;
    runWineTask(selectedValue, "winetricks", *wineTricksProcess, getUserConfigDirectory());
}

void MainWindow::runWineConfig(){
    QString selectedValue = getGameFile(*comboBox);
    qDebug() << selectedValue;
    runWineTask(selectedValue, "winecfg", *wineTricksProcess, getUserConfigDirectory());
}
void MainWindow::comboBoxCurrentIndexChanged(int index){
    updateCommandTextEdit(*comboBox, *commandTextEdit);
}
void MainWindow::killProcessFn(){
    killAppProcess(*killProcess);
}

void MainWindow::popUpWindowExec(){
    PopupWindow popupWindow(centralWidget);
    popupWindow.exec();
}
void MainWindow::runGameProcessFn(){
    QString selectedValue = getGameFile(*comboBox);
    QString commandText = commandTextEdit->toPlainText();
    runGameProcess(*gameProcess, commandText, selectedValue);
}
