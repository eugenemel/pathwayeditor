#include "mainwindow.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	MainWindow* mainWindow = new MainWindow();
    mainWindow->show();

    int rv = app.exec();
    return rv;
}
