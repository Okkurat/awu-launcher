#include "app_edit.h"
#include <QApplication>
#include <QComboBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QFileDialog>
#include <QDir>
#include <QLabel>
#include <QIcon>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QDialog>
#include "main_window.h"

QString getGameFile(QComboBox &comboBox){
        QVariantMap appName = comboBox.property("appName").value<QVariantMap>();
        QString selectedName = comboBox.currentText();
        QString selectedValue = appName.value(selectedName).toString();
        return selectedValue;
}
void runWineTask(const QString &selectedFile, const QString &taskName, QProcess &process, QString &userConfigDir) {
    if (selectedFile.isEmpty()) {
        qDebug() << "No game selected";
        return;
    }

    qDebug() << "Selected game:" << selectedFile;

    QDir directory(userConfigDir + "/awu/umu-conf");
    QString filePath = directory.absoluteFilePath(selectedFile);

    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString fileContent = in.readAll();
        file.close();

        int prefixPos = fileContent.indexOf("prefix");

        QString prefixPath = fileContent.mid(prefixPos).split('"')[1];
        qDebug() << "Prefix path:" << prefixPath;

        QString command = taskName;
        QString winePrefix = "WINEPREFIX=" + prefixPath;
        process.setWorkingDirectory(QDir::homePath());
        QStringList environment = QProcess::systemEnvironment();
        environment << QString("WINEPREFIX=%1").arg(prefixPath);
        process.setEnvironment(environment);
        process.start(command, QStringList());

        if (!process.waitForStarted()) {
            qDebug() << "Failed to open" << taskName;
            return;
        }
        qDebug() << taskName << "process started. Please close" << taskName << "manually when finished.";
    } else {
        qDebug() << "Failed to open file:" << filePath;
    }
}

QString getUserConfigDirectory() {
    QString configDir;
#ifdef XDG_CONFIG_HOME
    // Use the XDG_CONFIG_HOME environment variable if available
    configDir = QString::fromUtf8(qgetenv("XDG_CONFIG_HOME"));
#else
    QString homeDir = QDir::homePath();
    configDir = homeDir + "/.config";
#endif
    return configDir;
}

void createMyAppDirectory(const QString &userDir) {
    QDir().mkpath(userDir + "/awu");
    QDir().mkpath(userDir + "/awu/umu-conf");
    QDir().mkpath(userDir + "/awu/proton");
}

void populateComboBox(QComboBox &comboBox){
    QDir directory(getUserConfigDirectory() + "/awu/umu-conf");
    QStringList files = directory.entryList(QStringList() << "*.toml", QDir::Files);
    comboBox.clear();
    QMap<QString, QString> appName;
    foreach(const QString &file, files) {
        QString fileName = file.left(file.lastIndexOf('.'));
        QString filePath = directory.absoluteFilePath(file);

        // Read toml file
        QFile configFile(filePath);
        if(configFile.open(QIODevice::ReadOnly | QIODevice::Text)){
            QTextStream in(&configFile);
            while(!in.atEnd()){
                QString line = in.readLine().trimmed();
                if(line.startsWith("name")){
                    QString name = line.split('=')[1].trimmed();
                    name.remove(QChar('\"'));

                    appName.insert(name, fileName + ".toml");
                    comboBox.addItem(name);
                    break;
                }
            }
        }
        configFile.close();
    }
    comboBox.setCurrentIndex(-1);
    comboBox.setProperty("appName", QVariant::fromValue(appName));
}
QString cleanOutput(const QString &output) {
    QString cleanedOutput = output;
    cleanedOutput.remove(QRegExp("\x1B\\[[0-9;]*[A-Za-z]")); // Remove escape sequences
    cleanedOutput.remove(QRegExp("\"")); // Remove extra quotes
    cleanedOutput.replace("\\n", "\n"); // Replace "\\n" with newline
    return cleanedOutput.trimmed(); // Remove leading and trailing whitespace
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    // Check if the directory exists, if not, create it
    QString userConfigDir = getUserConfigDirectory();
    QDir myAppDir(userConfigDir + "/awu");
    if (!myAppDir.exists()) {
        createMyAppDirectory(userConfigDir);
    }

    QWidget window;

    QComboBox comboBox;
    QTextEdit commandTextEdit(&window);
    QLabel launchArgumentsLabel("Launch Arguments", &window);
    launchArgumentsLabel.setFixedSize(400, 30);
    commandTextEdit.setFixedSize(400, 30);
    QPushButton button("Play");
    button.setFixedSize(200, 30);
    QPushButton killButton("Kill Process");
    killButton.setFixedSize(200, 30);
    QPushButton wineTricksButton("Winetricks");
    wineTricksButton.setFixedSize(200,30);
    QPushButton wineConfigButton("Winecfg");
    wineConfigButton.setFixedSize(200,30);
    QPushButton addGameButton("Add game");
    addGameButton.setFixedSize(200,30);

    QVBoxLayout mainLayout(&window);


    QHBoxLayout buttonLayout;
    buttonLayout.addWidget(&button);
    buttonLayout.addWidget(&killButton);

    QHBoxLayout wineLayout;
    wineLayout.addWidget(&wineTricksButton);
    wineLayout.addWidget(&wineConfigButton);

    mainLayout.addWidget(&comboBox);
    mainLayout.addWidget(&launchArgumentsLabel);
    mainLayout.addWidget(&commandTextEdit);
    mainLayout.addLayout(&buttonLayout);
    mainLayout.addLayout(&wineLayout);
    mainLayout.addWidget(&addGameButton);

    QProcess gameProcess;
    QProcess killProcess;
    QProcess wineTricksProcess;
    QProcess wineConfigProcess;

    QObject::connect(&addGameButton, &QPushButton::clicked, [&](){
        PopupWindow popupWindow(&window);
        popupWindow.exec();
    });

    QObject::connect(&wineConfigButton, &QPushButton::clicked, [&]() {
        //
        //QVariantMap appName = comboBox.property("appName").value<QVariantMap>();
        //QString selectedName = comboBox.currentText();
        //QString selectedValue = appName.value(selectedName).toString();
        QString selectedValue = getGameFile(comboBox);
        qDebug() << selectedValue;

        runWineTask(selectedValue, "winecfg", wineConfigProcess, userConfigDir);
    });

    // Connect the winetricks button
    QObject::connect(&wineTricksButton, &QPushButton::clicked, [&]() {
        QString selectedValue = getGameFile(comboBox);
        qDebug() << selectedValue;
        runWineTask(selectedValue, "winetricks", wineTricksProcess, userConfigDir);
    });

    QObject::connect(&killButton, &QPushButton::clicked, [&]() {
        QString command = "ls -l /proc/*/exe 2>/dev/null | grep -E 'wine(64)?-preloader|wineserver' | perl -pe 's;^.*/proc/(\\d+)/exe.*$;$1;g;' | xargs -n 1 kill | killall -s9 winedevice.exe";

        // Start the process to run the command
        killProcess.start("/bin/bash", QStringList() << "-c" << command);

        // Wait for the process to finish
        if (!killProcess.waitForFinished()) {
            qDebug() << "Error: Failed to execute the command.";
        } else {
            qDebug() << "Command executed successfully.";
        }
    });


    QObject::connect(&comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), [&](int index){
        QDir directory(userConfigDir + "/awu/umu-conf");
        if (index >= 0) {
            QString fileName = comboBox.itemText(index);
            QString debugFileName = getGameFile(comboBox);
            QFile configFile(directory.absoluteFilePath(debugFileName));
            if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&configFile);
                while(!in.atEnd()){
                    QString line = in.readLine().trimmed();
                    if(line.startsWith("awu_args")){
                        QString args = line.split('=')[1].trimmed();
                        args.remove(QChar('\"'));
                        commandTextEdit.clear();
                        if(args != "none"){
                            commandTextEdit.insertPlainText(args);
                        }
                        break;
                }
                }
            } else {
                commandTextEdit.clear();
            }
        }
    });

    QObject::connect(&button, &QPushButton::clicked, [&]() {
        QStringList arguments;
        gameProcess.setWorkingDirectory(userConfigDir + "/awu/umu-conf");
        QString commandText = commandTextEdit.toPlainText();
        qDebug() << commandText;
        QStringList commandParts = commandText.split(' ', Qt::SkipEmptyParts);
        QString selectedValue = getGameFile(comboBox);


        if (!commandTextEdit.toPlainText().isEmpty()) {
            QString command = commandParts.takeFirst();
            arguments << commandParts;
            arguments << "umu-run" << "--config" << selectedValue;
            gameProcess.start(command, arguments);
        } else {
            arguments << "--config" << selectedValue;
            gameProcess.start("umu-run", arguments);
        }
    });

    QObject::connect(&wineConfigProcess, &QProcess::readyReadStandardOutput, [&]() {
        QByteArray output = wineTricksProcess.readAllStandardOutput();
        qDebug() << cleanOutput(output);
    });

    QObject::connect(&wineConfigProcess, &QProcess::readyReadStandardError, [&]() {
        QByteArray error = wineTricksProcess.readAllStandardError();
        qDebug() << cleanOutput(error);
    });

    QObject::connect(&wineConfigProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit) {
            qDebug() << "Process finished successfully with exit code:" << exitCode;
        } else {
            qDebug() << "Process finished with error exit code:" << exitCode;
        }
    });


    QObject::connect(&wineTricksProcess, &QProcess::readyReadStandardOutput, [&]() {
        QByteArray output = wineTricksProcess.readAllStandardOutput();
        qDebug() << cleanOutput(output);
    });

    QObject::connect(&wineTricksProcess, &QProcess::readyReadStandardError, [&]() {
        QByteArray error = wineTricksProcess.readAllStandardError();
        qDebug() << cleanOutput(error);
    });

    QObject::connect(&wineTricksProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit) {
            qDebug() << "Process finished successfully with exit code:" << exitCode;
        } else {
            qDebug() << "Process finished with error exit code:" << exitCode;
        }
    });

    QObject::connect(&gameProcess, &QProcess::readyReadStandardOutput, [&]() {
        QByteArray output = gameProcess.readAllStandardOutput();
        qDebug() << cleanOutput(output);
    });

    QObject::connect(&gameProcess, &QProcess::readyReadStandardError, [&]() {
        QByteArray error = gameProcess.readAllStandardError();
        qDebug() << cleanOutput(error);
    });


    QObject::connect(&gameProcess, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit) {
            qDebug() << "Process finished successfully with exit code:" << exitCode;
        } else {
            qDebug() << "Process finished with error exit code:" << exitCode;
        }
    });

    populateComboBox(comboBox);
    commandTextEdit.clear();

    window.setLayout(&mainLayout);
    window.setWindowTitle("Awu");
    window.show();
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}