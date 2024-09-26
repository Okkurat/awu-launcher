#include <QApplication>
#include "main_window.h"
#include "utils.h"
int main(int argc, char **argv){
    
    QApplication app(argc, argv);
    createMyAppDirectory();
    MainWindow mainWindow;
    mainWindow.show();
    return app.exec();
}
