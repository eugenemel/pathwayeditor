#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {

   pathwayWidget = new PathwayWidget(this);
   setCentralWidget(pathwayWidget);
   pathwayWidget->showGraph();
}
