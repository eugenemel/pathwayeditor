#ifndef  MAINWINDOW_INCLUDED
#define  MAINWINDOW_INCLUDED

#include "stable.h"
#include "pathwaywidget.h"
#include "tabledockwidget.h"

class PathwayWidget;
class TableDockWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
	PathwayWidget* pathwayWidget;
    TableDockWidget* spreadsheetWidget;

    PathwayWidget* getPathwayWdiget() { return pathwayWidget; }
    TableDockWidget* getSpreadsheet() { return spreadsheetWidget;   }
};

#endif
