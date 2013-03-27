#include "mainwindow.h"
MainWindow::MainWindow(QWidget *parent): QMainWindow(parent) {

   spreadsheetWidget = new TableDockWidget(this);
   pathwayWidget = new PathwayWidget(this);

   addDockWidget(Qt::BottomDockWidgetArea,spreadsheetWidget,Qt::Horizontal);
   setCentralWidget(pathwayWidget);
   pathwayWidget->showGraph();

}
