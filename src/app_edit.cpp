// app_edit.cpp
#include "app_edit.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QDebug>
#include <QWidget>
#include <QIcon>
#include <QComboBox>
PopupWindow::PopupWindow(QWidget *parent) : QDialog(parent) {
    	
    centralWidget = new QWidget(this);
	
	layout = new QVBoxLayout(centralWidget);
	doneCloseLayout = new QHBoxLayout();
	gameNameLayout = new QHBoxLayout();
	prefixLayout = new QHBoxLayout();
	gameExeLayout = new QHBoxLayout();


	gameLabel = new QLabel("Game name");
	prefixLabel = new QLabel("Choose prefix");
	gameName = new QTextEdit();
	prefixPath = new QTextEdit();
	doneButton = new QPushButton("Done");
    closeButton = new QPushButton("Close");
	prefixButton = new QPushButton();
    searchButton = new QPushButton();
	protonComboBox = new QComboBox();
	protonLabel = new QLabel("Choose proton");
	gameIdLabel = new QLabel("Choose game id (if left empty, default is 0)");
	gameIdEdit = new QTextEdit();
	gameExeLabel = new QLabel("Choose executable");
	gameExeEdit = new QTextEdit();
	gameExeButton = new QPushButton();
	launchArgsLabel = new QLabel("Choose launch arguments (Proton/Wine related)");
	launchArgsEdit = new QTextEdit();
	storeLabel = new QLabel("Choose store");
	storeComboBox = new QComboBox();
	awuArgsLabel = new QLabel("Awu arguments (for stuff like gamescope)");
	awuArgsEdit = new QTextEdit();

	prefixButton->setIcon(QIcon::fromTheme("folder"));
	searchButton->setIcon(QIcon::fromTheme("edit-find"));
	gameExeButton->setIcon(QIcon::fromTheme("folder"));
	gameLabel->setFixedSize(400,30);
	gameName->setFixedSize(400,30);
	prefixLabel->setFixedSize(400,30);
	prefixPath->setFixedSize(400,30);
	protonLabel->setFixedSize(400,30);
	gameIdLabel->setFixedSize(400,30);
	gameIdEdit->setFixedSize(400,30);
	gameExeLabel->setFixedSize(400,30);
	gameExeEdit->setFixedSize(400,30);
	launchArgsLabel->setFixedSize(400,30);
	launchArgsEdit->setFixedSize(400,30);
	storeLabel->setFixedSize(400,30);	
	awuArgsLabel->setFixedSize(400,30);
	awuArgsEdit->setFixedSize(400,30);


	storeComboBox->addItem("Amazon");
	storeComboBox->addItem("Epic Games Store");
	storeComboBox->addItem("GOG");
	storeComboBox->addItem("HumbleBundle");
	storeComboBox->addItem("Ubisoft");
	storeComboBox->addItem("None");
	storeComboBox->addItem("ZOOM Platform");

	doneCloseLayout->addWidget(doneButton);
	doneCloseLayout->addWidget(closeButton);

	gameNameLayout->addWidget(gameName);
	gameNameLayout->addWidget(searchButton);

	prefixLayout->addWidget(prefixPath);
	prefixLayout->addWidget(prefixButton);

	gameExeLayout->addWidget(gameExeEdit);
	gameExeLayout->addWidget(gameExeButton);

	layout->addLayout(doneCloseLayout);
	layout->addWidget(gameLabel);
	layout->addLayout(gameNameLayout);
	layout->addWidget(prefixLabel);
	layout->addLayout(prefixLayout);
	layout->addWidget(protonLabel);
	layout->addWidget(protonComboBox);
	layout->addWidget(gameIdLabel);
	layout->addWidget(gameIdEdit);
	layout->addWidget(gameExeLabel);
	layout->addLayout(gameExeLayout);
	layout->addWidget(launchArgsLabel);
	layout->addWidget(launchArgsEdit);
	layout->addWidget(storeLabel);
	layout->addWidget(storeComboBox);
	layout->addWidget(awuArgsLabel);
	layout->addWidget(awuArgsEdit);


	resize(500,700);

    connect(closeButton, &QPushButton::clicked, this, &PopupWindow::close);
	connect(doneButton, &QPushButton::clicked, this, &PopupWindow::doneFn);
	connect(searchButton, &QPushButton::clicked, this, &PopupWindow::searchDatabase);
	connect(prefixButton, &QPushButton::clicked, this, &PopupWindow::setPrefix);
	connect(gameExeButton, &QPushButton::clicked, this, &PopupWindow::setExe);	
}

PopupWindow::~PopupWindow() {
}
void PopupWindow::doneFn(){
	qDebug() << "done button pressed";
	PopupWindow::close();
}
void PopupWindow::searchDatabase(){
	QString gameTitle = gameName->toPlainText();
	qDebug() << "search button pressed";
	qDebug() << gameTitle;
}
void PopupWindow::setPrefix(){
	qDebug() << "setPrefix function";
}
void PopupWindow::setExe(){
	qDebug() << "SetExe function";
}
