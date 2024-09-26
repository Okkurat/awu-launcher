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
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    gameProcess = new QProcess(this);
    killProcess = new QProcess(this);
    wineTricksProcess = new QProcess(this);
    wineConfigProcess = new QProcess(this);
    comboBox = new QComboBox(this);
    launchArgumentsLabel = new QLabel("Launch arguments", this);
    commandTextEdit = new QTextEdit(this);
    playButton = new QPushButton("Play");
    killButton = new QPushButton("Kill Process");
    wineTricksButton = new QPushButton("Winetricks");
    wineConfigButton = new QPushButton("WineConfig");
    addGameButton = new QPushButton("Add game");
	deleteGameButton = new QPushButton("Delete game");
	editGameButton = new QPushButton("Edit");

    launchArgumentsLabel->setFixedSize(400,30);
    commandTextEdit->setFixedSize(400, 30);
    playButton->setFixedSize(200,30);
    wineTricksButton->setFixedSize(200,30);
    killButton->setFixedSize(200,30);
    wineConfigButton->setFixedSize(200,30);
    addGameButton->setFixedSize(200,30);
	deleteGameButton->setFixedSize(200,30);

    mainLayout = new QVBoxLayout(centralWidget);
    comboBoxLayout = new QHBoxLayout;
    comboBoxLayout->addWidget(comboBox);
    comboBoxLayout->addWidget(editGameButton);

    mainLayout->addLayout(comboBoxLayout);
    mainLayout->addWidget(launchArgumentsLabel);
    mainLayout->addWidget(commandTextEdit);

    wineLayout = new QHBoxLayout;
    wineLayout->addWidget(wineTricksButton);
    wineLayout->addWidget(wineConfigButton);

    mainButtonLayout = new QHBoxLayout;
    mainButtonLayout->addWidget(playButton);
    mainButtonLayout->addWidget(killButton);
	

	addDeleteLayout = new QHBoxLayout;
	addDeleteLayout->addWidget(addGameButton);
	addDeleteLayout->addWidget(deleteGameButton);

	
    mainLayout->addWidget(addGameButton);
	mainLayout->addWidget(deleteGameButton);
	
    mainLayout->addLayout(mainButtonLayout);
    mainLayout->addLayout(wineLayout);
	mainLayout->addLayout(addDeleteLayout);

    populateComboBox(*comboBox);
    commandTextEdit->clear();

    setWindowTitle("Awu");

    connect(addGameButton, &QPushButton::clicked, this, &MainWindow::addGameWindowExec);
    connect(wineTricksButton, &QPushButton::clicked, this, &MainWindow::runWineTricks);
    connect(wineConfigButton, &QPushButton::clicked, this, &MainWindow::runWineConfig);
    connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::comboBoxCurrentIndexChanged);
    connect(killButton, &QPushButton::clicked, this, &MainWindow::killProcessFn);
    connect(playButton, &QPushButton::clicked, this, &MainWindow::runGameProcessFn);
	connect(deleteGameButton, &QPushButton::clicked, this, &MainWindow::deleteGameFn);
    connect(editGameButton, &QPushButton::clicked, this, &MainWindow::editGameFn);
}


MainWindow::~MainWindow() {
}
void MainWindow::editGameFn(){
    QString gameFile = getGameInfo(*comboBox).first;
    if(gameFile.isEmpty()){
        return;
    }
    PopupWindow editGamePopupWindow(centralWidget, gameFile);
    int result = editGamePopupWindow.exec();
    populateComboBox(*comboBox);
    commandTextEdit->clear();
}

void MainWindow::runWineTricks(){
    QString selectedValue = getGameInfo(*comboBox).first;
    qDebug() << selectedValue;
    runWineTask(selectedValue, "winetricks", *wineTricksProcess, getUserConfigDirectory());
}

void MainWindow::runWineConfig(){
    QString selectedValue = getGameInfo(*comboBox).first;
    qDebug() << selectedValue;
    runWineTask(selectedValue, "winecfg", *wineConfigProcess, getUserConfigDirectory());
}
void MainWindow::comboBoxCurrentIndexChanged(int index){
    updateCommandTextEdit(*comboBox, *commandTextEdit);
}
void MainWindow::killProcessFn(){
    killAppProcess(*killProcess);
}

void MainWindow::deleteGameFn(){
	QPair<QString, QString> gameInfoPair = getGameInfo(*comboBox);
    QString selectedValue = gameInfoPair.first;
    QString appName = gameInfoPair.second;
	
	qDebug() << appName;

	QMessageBox::StandardButton dialog;
	dialog = QMessageBox::question(nullptr, "Confirmation", "Do you want to delete <b>" + appName + "</b>?<br> <font color=\"red\">Note that this only deletes config file, not the prefix nor game files themselves</font>", QMessageBox::Yes|QMessageBox::No);

	if(dialog == QMessageBox::Yes){
		qDebug() << "Yes clicked";
	} else {
		qDebug() << "No clicked";
		return;
	}

	bool deletedGame = deleteGame(selectedValue);
	if(deletedGame){
		populateComboBox(*comboBox);
		commandTextEdit->clear();
	}
}


void MainWindow::addGameWindowExec(){
    PopupWindow popupWindow(centralWidget);
    int result = popupWindow.exec();
    populateComboBox(*comboBox);
    commandTextEdit->clear();
}
void MainWindow::runGameProcessFn(){
    QString selectedValue = getGameInfo(*comboBox).first;
    QString commandText = commandTextEdit->toPlainText();
    runGameProcess(*gameProcess, commandText, selectedValue);
}
