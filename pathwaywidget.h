#ifndef PATHWAY_VIEW
#define PATHWAY_VIEW

#include "graphwidget.h"
#include "tabledockwidget.h"

class MainWindow;
class TableDockWidget;

class PathwayWidget: public QDockWidget {
      Q_OBJECT

public:
      PathwayWidget(MainWindow*);
      void showGraph();

public slots:
      void pause();
      void run();
	  void saveModelFile();
	  void loadModelFile();

protected:
      void timerEvent(QTimerEvent *);
      void setAnimationSpeed(int speed);


private:
      void addToolBar();
      MainWindow* _mw;
      GraphWidget* _graph;

      int timerId;
      
};

#endif
