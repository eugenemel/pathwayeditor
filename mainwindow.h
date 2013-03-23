#ifndef  MAINWINDOW_INCLUDED
#define  MAINWINDOW_INCLUDED

#include "stable.h"
#include "pathwaywidget.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
	PathwayWidget* pathwayWidget;
};

#endif
