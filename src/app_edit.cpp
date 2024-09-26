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
#include "utils.h"
#include <QDir>
#include <QFileDialog>
#include <QDirIterator>
#include <QMessageBox>

PopupWindow::PopupWindow(QWidget *parent, const QString& gameFile) : QDialog(parent) {
    	
    centralWidget = new QWidget(this);
	
    qDebug() << "Gamefile: " << gameFile;

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
    gameIdLabel = new QLabel("Choose game id (default should be 0)");
    gameIdEdit = new QTextEdit("0");
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


    storeComboBox->addItem("amazon");
    storeComboBox->addItem("battlenet");
    storeComboBox->addItem("ea");
    storeComboBox->addItem("egs");
    storeComboBox->addItem("gog");
    storeComboBox->addItem("humble");
    storeComboBox->addItem("itchio");
    storeComboBox->addItem("ubisoft");
    storeComboBox->addItem("zoomplatform");
    storeComboBox->addItem("none");
    storeComboBox->setCurrentIndex(9);

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
    connect(doneButton, &QPushButton::clicked, this, [=]() { doneFn(gameFile); });
    connect(searchButton, &QPushButton::clicked, this, &PopupWindow::searchDatabase);
    connect(prefixButton, &QPushButton::clicked, this, &PopupWindow::setPrefix);
    connect(gameExeButton, &QPushButton::clicked, this, &PopupWindow::setExe);
    connect(protonComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &PopupWindow::protonComboBoxChanged);

    populateProtonBox();

    if (!gameFile.isEmpty()) {
        int index;
        QString filePath = getUserConfigDirectory() + "/awu/umu-conf/" + gameFile;
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                qDebug() << line;

                QStringList parts = line.split("=");
                if(parts.length() == 2){
                    QString key = parts[0].trimmed();
                    QString value = parts[1].trimmed().remove('\"');
                if(key == "prefix"){
                    prefixPath->setText(value);
                }
                else if(key == "game_id"){
                    gameIdEdit->setText(value);
                }
                else if(key == "launch_args"){
                    value = value.remove('[').remove(']');
                    QStringList args = value.split(",", Qt::SkipEmptyParts);
                    QString launchArgs;
                    for (const QString &arg : args)
                        launchArgs += arg.trimmed() + " ";
                        launchArgsEdit->setText(launchArgs.trimmed());
                    }
                else if(key == "store"){
                    index = storeComboBox->findText(value);
                    if (index != -1){
                        storeComboBox->setCurrentIndex(index);
                    }
                }
                else if(key == "proton"){
                    QStringList pathParts = value.split('/');
                    QString protonName = pathParts.last();
                    qDebug() << protonName;
                    index = protonComboBox->findText(protonName);
                    if (index != -1){
                        protonComboBox->setCurrentIndex(index);
                    }
                }
                else if(key == "name"){
                    gameName->setText(value);
                }
                else if(key == "awu_args"){
                    awuArgsEdit->setText(value);
                }
                else if(key == "exe"){
                    gameExeEdit->setText(value);
                }
            }

            }
        file.close();
    } else {
        qDebug() << "Failed to open file";
    }
    }

}

PopupWindow::~PopupWindow() {
}

void PopupWindow::doneFn(const QString& gameFile){
    qDebug() << "Done button pressed";

    QString gameTitle = gameName->toPlainText().trimmed();
    gameTitle = QString("%1 = \"%2\"").arg("name").arg(gameTitle);

    QString prefixDir = prefixPath->toPlainText().trimmed();
    prefixDir = QString("%1 = \"%2\"").arg("prefix").arg(prefixDir);

    QString gameId = gameIdEdit->toPlainText().trimmed();
    gameId = QString("%1 = \"%2\"").arg("game_id").arg(gameId);

    QString gameExe = gameExeEdit->toPlainText().trimmed();
    gameExe = QString("%1 = \"%2\"").arg("exe").arg(gameExe);
    QString launchArgsTemp;
    QString launchArgs = launchArgsEdit->toPlainText().trimmed();
    if(launchArgs.isEmpty()) {
        launchArgsTemp = "[\"\"]";
    } else {
        QStringList launchArgsList = launchArgs.split(" ", Qt::SkipEmptyParts);
        launchArgsTemp = "[";
        for(const QString &arg: launchArgsList) {
            launchArgsTemp += "\"";
            launchArgsTemp += arg;
            launchArgsTemp += "\",";
        }
        launchArgsTemp.replace(launchArgsTemp.length() - 1, 1, "]");
	}
    launchArgs = QString("%1 = %2").arg("launch_args").arg(launchArgsTemp);

    QString awuArgs = awuArgsEdit->toPlainText().trimmed();
    awuArgs = QString("%1 = \"%2\"").arg("awu_args").arg(awuArgs);
    QString selectedProton = protonComboBox->currentData().toString().trimmed();
    selectedProton = QString("%1 = \"%2\"").arg("proton").arg(selectedProton);
    QString selectedStore = storeComboBox->currentText().trimmed();
    selectedStore = QString("%1 = \"%2\"").arg("store").arg(selectedStore);
    QList<QString> config;
    config.append(prefixDir);
    config.append(selectedProton);
    config.append(gameId);
    config.append(gameExe);
    config.append(launchArgs);
    config.append(selectedStore);
    config.append(gameTitle);
    config.append(awuArgs);
    QString file;
    if(gameFile.isEmpty()){
        QDateTime currentDateTime = QDateTime::currentDateTime();
        qint64 unixTime = currentDateTime.toSecsSinceEpoch();
        file = QString::number(unixTime) + ".toml";
    } else {
        file = gameFile;
    }
    bool fileWrite = writeConfigFile(config, file);
    if(fileWrite){
        PopupWindow::close();
    } else {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("An error occurred!");
        msgBox.setInformativeText("Couldnt write to file, check terminal window");
        msgBox.setWindowTitle("Error");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        PopupWindow::close();
    }
}
void PopupWindow::searchDatabase(){
    QString gameTitle = gameName->toPlainText();
    qDebug() << "search button pressed";
    qDebug() << gameTitle;
}
void PopupWindow::setPrefix(){
    qDebug() << "setPrefix function";
    QString prefixDir = QFileDialog::getExistingDirectory(this, "Select Prefix Directory", QDir::homePath());
    if (!prefixDir.isEmpty()) {
        qDebug() << "Selected Prefix Directory: " << prefixDir;
        prefixPath->setText(prefixDir);
    }
}
void PopupWindow::setExe(){
    qDebug() << "SetExe function";
    QString exeFile = QFileDialog::getOpenFileName(this, "Select Executable File", QDir::homePath());
    if (!exeFile.isEmpty()) {
        qDebug() << "Selected Executable File: " << exeFile;
        gameExeEdit->setText(exeFile);
    }
}
void PopupWindow::populateProtonBox() {
    qDebug() << "populateProtonBox";
    QDir ProtonDir(getUserConfigDirectory() + "/awu/proton");
    if (!ProtonDir.exists()) {
        qDebug() << "Proton directory does not exist!";
        return;
    }

    QStringList filters;
    filters << "*";

    QStringList protonNames;
    QDirIterator it(ProtonDir.absolutePath(), filters, QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
    while (it.hasNext()) {
        it.next();
        QString protonName = it.fileName();
        QString protonPath = it.filePath();
        qDebug() << "Proton found: " << protonName << " Path: " << protonPath;
        protonComboBox->addItem(protonName, protonPath);
    }
    protonComboBox->setCurrentIndex(-1);
}



void PopupWindow::protonComboBoxChanged(int index) {
    QString selectedProton = protonComboBox->currentText();
    QString selectedProtonPath = protonComboBox->currentData().toString();
    qDebug() << "Selected Proton: " << selectedProton;
    qDebug() << "Path: " << selectedProtonPath;
}
