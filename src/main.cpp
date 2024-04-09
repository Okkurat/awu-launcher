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
#include "utils.h"


int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    createMyAppDirectory();

    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
