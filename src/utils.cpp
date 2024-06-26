#include "utils.h"
#include <QDir>
#include <QComboBox>
#include <QTextStream>
#include <QProcess>
#include <QDebug>
#include <QTextEdit>
#include <QFile>
#include <QList>
#include <fstream>
#include <sstream>
#include <string>

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

void createMyAppDirectory() {
    QDir AppDir(getUserConfigDirectory() + "/awu");
    QDir ProtonDir(getUserConfigDirectory() + "/awu/proton");
    QDir ConfDir(getUserConfigDirectory() + "/awu/umu-conf");

    if(!AppDir.exists()){
        QDir().mkpath(getUserConfigDirectory() + "/awu");
    }
    if(!ConfDir.exists()){
        QDir().mkpath(getUserConfigDirectory() + "/awu/umu-conf");
    }
    if(!ProtonDir.exists()){
        QDir().mkpath(getUserConfigDirectory() + "/awu/proton");
    }
    return;
}
void populateComboBox(QComboBox &comboBox){
    QDir directory(getUserConfigDirectory() + "/awu/umu-conf");
    QStringList files = directory.entryList(QStringList() << "*.toml", QDir::Files);
    comboBox.clear();
    QMap<QString, QString> appName;
    foreach(const QString &file, files) {
        QString fileName = file.left(file.lastIndexOf('.'));
        QString filePath = directory.absoluteFilePath(file);

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

void runWineTask(const QString &selectedFile, const QString &taskName, QProcess &process, const QString &userConfigDir) {
    if (selectedFile.isEmpty()) {
        qDebug() << "No game selected";
        return;
    }

    qDebug() << "Selected game:" << selectedFile;
    qDebug () << userConfigDir;
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

        QObject::connect(&process, &QProcess::readyReadStandardOutput, [&]() {
            QByteArray output = process.readAllStandardOutput();
            qDebug() << cleanOutput(output);
        });

        QObject::connect(&process, &QProcess::readyReadStandardError, [&]() {
            QByteArray error = process.readAllStandardError();
            qDebug() << cleanOutput(error);
        });

        QObject::connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](int exitCode, QProcess::ExitStatus exitStatus) {
            if (exitStatus == QProcess::NormalExit) {
                qDebug() << "Process finished successfully with exit code:" << exitCode;
            } else {
                qDebug() << "Process finished with error exit code:" << exitCode;
            }
        });
    } else {
        qDebug() << "Failed to open file:" << filePath;
    }
}
QString cleanOutput(const QString &output) {
    QString cleanedOutput = output;
    QRegularExpression re("\x1B\\[[0-9;]*[A-Za-z]");
    cleanedOutput.remove(re);
    cleanedOutput.remove("\"");
    cleanedOutput.replace("\\n", "\n");
    return cleanedOutput.trimmed();
}

QPair<QString, QString> getGameInfo(QComboBox &comboBox){
    QVariantMap appNameMap = comboBox.property("appName").toMap();
    QString selectedName = comboBox.currentText();
    QString selectedValue = appNameMap[selectedName].toString();
    return qMakePair(selectedValue, selectedName);
}

void runGameProcess(QProcess &process, QString commandText, QString selectedGame){

    QStringList arguments;
    QString umuRun;
    process.setWorkingDirectory(getUserConfigDirectory() + "/awu/umu-conf");
    qDebug() << commandText;
    QStringList commandParts = commandText.split(' ', Qt::SkipEmptyParts);

    // Currently if umu-run is not /usr, supports launching through /.local/bin only
    // Some weird behaviour with paths happen when you run this as desktop file. If you give commands like mangohud and / or gamescope it works, but otherwise it will not find the path so we do this the ugly way
    QString umuRunPathTest = QDir::homePath() + "/.local/bin/umu-run";
    QFile umuRunPathCheck(umuRunPathTest);
    if(umuRunPathCheck.exists()){
        umuRun = umuRunPathTest;
    } else {
        umuRun = "umu-run";
    }
    qDebug() << umuRun;
    if (!commandText.isEmpty()) {
        QString command = commandParts.takeFirst();
        arguments << commandParts;
        arguments << umuRun << "--config" << selectedGame;
        process.start(command, arguments);
    } else {
        arguments << "--config" << selectedGame;
        process.start(umuRun, arguments);
    }
    if (!process.waitForStarted()) {
        qDebug() << "Failed to start process:" << process.errorString();
        return;
    }
    QObject::connect(&process, &QProcess::readyReadStandardOutput, [&]() {
        QByteArray output = process.readAllStandardOutput();
        qDebug() << cleanOutput(output);
    });
    QObject::connect(&process, &QProcess::readyReadStandardError, [&]() {
        QByteArray error = process.readAllStandardError();
        qDebug() << cleanOutput(error);
    });
    QObject::connect(&process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), [&](int exitCode, QProcess::ExitStatus exitStatus) {
        if (exitStatus == QProcess::NormalExit) {
            qDebug() << "Process finished successfully with exit code:" << exitCode;
        } else {
            qDebug() << "Process finished with error exit code:" << exitCode;
        }
    });
}
void killAppProcess(QProcess &process){
    QString command = "ls -l /proc/*/exe 2>/dev/null | grep -E 'wine(64)?-preloader|wineserver' | perl -pe 's;^.*/proc/(\\d+)/exe.*$;$1;g;' | xargs -n 1 kill | killall -s9 winedevice.exe";

    process.start("/bin/bash", QStringList() << "-c" << command);

    if (!process.waitForFinished()) {
        qDebug() << "Error: Failed to execute the command.";
    } else {
        qDebug() << "Command executed successfully.";
    }
}
void updateCommandTextEdit(QComboBox &comboBox, QTextEdit &commandTextEdit) {
    QDir directory(getUserConfigDirectory() + "/awu/umu-conf");
    int index = comboBox.currentIndex();
    if (index >= 0) {
        QString fileName = comboBox.itemText(index);
        QString debugFileName = getGameInfo(comboBox).first;
        QFile configFile(directory.absoluteFilePath(debugFileName));
        if (configFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&configFile);
            while (!in.atEnd()) {
                QString line = in.readLine().trimmed();
                if (line.startsWith("awu_args")) {
                    QString args = line.split('=')[1].trimmed();
                    args.remove(QChar('\"'));
                    commandTextEdit.clear();
                    if (args != "none") {
                        commandTextEdit.insertPlainText(args);
                    }
                    break;
                }
            }
        } else {
            commandTextEdit.clear();
        }
    }
}
bool deleteGame(const QString &selectedFile){
	qDebug() << "Selected game: " << selectedFile;
	
	if(selectedFile.isEmpty()){
		return false;
	}
	QString filePath = getUserConfigDirectory() + "/awu/umu-conf/" + selectedFile;
	qDebug() << filePath;
	if(QFile::exists(filePath)){
		if(QFile::remove(filePath)){
			qDebug() << selectedFile << " Deleted succesfully";
			return true;
		} else {
			qDebug() << "Failed to delete the file";
			return false;
		}
	} else {
		qDebug() << "File does not exist";
		return false;
	}

	return true;
}
bool writeConfigFile(QList<QString> config, const QString &fileName){
    QString arg;
    QString filePath = getUserConfigDirectory() + "/awu/umu-conf/" + fileName;
    qDebug() << filePath;
    std::string str = filePath.toStdString();
    std::ofstream file(str);
    if(!file.is_open()) {
        qDebug() << "Failed to open file for writing";
        return false;
    }
    file << "# " << fileName.toStdString() << std::endl;
    qDebug() << "# " << fileName;
    file << "[umu]" << std::endl;
    qDebug() << "[umu]";
    for (int i = 0; i < config.size(); ++i) {
        qDebug() << config[i];
        file << config[i].toStdString() << std::endl;
        if(i == 5) {
            file << "[awu]" << std::endl;
            qDebug() << "[awu]";
        }
    }
    file.close();

    qDebug() << "Config was successfully written to the file: " << fileName;
    qDebug() << "File was written to: " << filePath;

    return true;
}
