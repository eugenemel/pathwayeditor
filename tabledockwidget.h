#ifndef TABLEDOCKWIDGET_H
#define TABLEDOCKWIDGET_H

#include "stable.h"
#include "mainwindow.h"
#include "numeric_treewidgetitem.h"
#include "node.h" 

class MainWindow;
class NumericTreeWidgetItem;
class Node;

using namespace std;

class TableDockWidget: public QDockWidget {
      Q_OBJECT

public:
    QWidget 	*dockWidgetContents;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;

    TableDockWidget(MainWindow* mw);
    ~TableDockWidget();
    void setGraphWidget(GraphWidget* g) { _graphWidget = g; }

public slots: 
       int loadSpreadsheet(QString fileName);
       int loadCSVFile(QString filename, QString sep);
       void updateTable();
       void addNode(Node*, QTreeWidgetItem* );
       void loadPeakTable();
	void switchTableView();
protected:
   	 void dragEnterEvent(QDragEnterEvent *event);
    	 void dropEvent(QDropEvent *event);
	
protected slots:
	  void keyPressEvent( QKeyEvent *e );
	  void contextMenuEvent ( QContextMenuEvent * event );
	  void updateItem(QTreeWidgetItem*);

private:
	  void setupTable();
          void heatmapBackground(QTreeWidgetItem* item);
          enum tableViewType{ nodeView=0, edgeView=1 };
          tableViewType viewType;
          
	MainWindow* _mainwindow;
	GraphWidget* _graphWidget;
};

#endif
