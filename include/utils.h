#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QComboBox>
#include <QProcess>
#include <QTextEdit>

QString getUserConfigDirectory();
void createMyAppDirectory();
void populateComboBox(QComboBox &comboBox);
void runWineTask(const QString &selectedFile, const QString &taskName, QProcess &process, const QString &userConfigDir);
QString cleanOutput(const QString &output);
QString getGameFile(QComboBox &comboBox);
void runGameProcess(QProcess &process, QString commandText, QString selectedGame);
void killAppProcess(QProcess &process);
void updateCommandTextEdit(QComboBox &comboBox, QTextEdit &commandTextEdit);

#endif // UTILS_H
