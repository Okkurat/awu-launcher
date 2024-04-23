#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QComboBox>
#include <QProcess>
#include <QTextEdit>
#include <QList>

QString getUserConfigDirectory();
void createMyAppDirectory();
void populateComboBox(QComboBox &comboBox);
void runWineTask(const QString &selectedFile, const QString &taskName, QProcess &process, const QString &userConfigDir);
QString cleanOutput(const QString &output);
QPair<QString, QString> getGameInfo(QComboBox &comboBox);
void runGameProcess(QProcess &process, QString commandText, QString selectedGame);
void killAppProcess(QProcess &process);
void updateCommandTextEdit(QComboBox &comboBox, QTextEdit &commandTextEdit);
bool deleteGame(const QString &selectedFile);
bool writeConfigFile(QList<QString> config, const QString &fileName);
#endif // UTILS_H
