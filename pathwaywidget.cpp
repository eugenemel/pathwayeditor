#include "pathwaywidget.h"

PathwayWidget::PathwayWidget(MainWindow* mw) { 
  _mw = mw;
  _graph = new GraphWidget();
  _graph->setLayoutAlgorithm(GraphWidget::Circular);
  _graph->setNodeSizeScale(3);
  _graph->setLabelSizeScale(2);
  setWidget(_graph);
  addToolBar();

  connect(_graph,SIGNAL(nodeCreated(Node*)), _mw->getSpreadsheet(), SLOT(addItem(Node*)));
  connect(_graph,SIGNAL(nodeDeleted(Node*)), _mw->getSpreadsheet(), SLOT(removeItem(Node*)));
  connect(_graph,SIGNAL(nodeChanged(Node*)), _mw->getSpreadsheet(), SLOT(updateItem(Node*)));

  timerId=0;
  setAnimationSpeed(100);
  pause();
}

void PathwayWidget::addToolBar() {
		QToolBar *toolBar = new QToolBar(this);
		toolBar->setFloatable(false);
		toolBar->setMovable(false);

		QToolButton *btnZoomIn = new QToolButton(toolBar);
		btnZoomIn->setIcon(QIcon(":images/zoomin.png"));
		connect(btnZoomIn, SIGNAL(clicked()), _graph, SLOT(zoomIn()));

		QToolButton *btnZoomOut = new QToolButton(toolBar);
		btnZoomOut->setIcon(QIcon(":/images/zoomout.png"));
		connect(btnZoomOut, SIGNAL(clicked()), _graph, SLOT(zoomOut()));

		QToolButton *btnTextZoomIn = new QToolButton(toolBar);
		btnTextZoomIn->setIcon(QIcon(":/images/zoomInText.png"));
		connect(btnTextZoomIn, SIGNAL(clicked()), _graph, SLOT(increaseLabelSize()));

		QToolButton *btnTextZoomOut = new QToolButton(toolBar);
		btnTextZoomOut->setIcon(QIcon(":/images/zoomOutText.png"));
		connect(btnTextZoomOut, SIGNAL(clicked()), _graph, SLOT(decreaseLabelSize()));

		QToolButton *btnNodeZoomIn = new QToolButton(toolBar);
		btnNodeZoomIn->setIcon(QIcon(":/images/zoomInNode.png"));
		connect(btnNodeZoomIn, SIGNAL(clicked()), _graph, SLOT(increaseNodeSize()));

		QToolButton *btnNodeZoomOut = new QToolButton(toolBar);
		btnNodeZoomOut->setIcon(QIcon(":/images/zoomOutNode.png"));
		connect(btnNodeZoomOut, SIGNAL(clicked()), _graph, SLOT(decreaseNodeSize()));

		QToolButton *btnEdgeZoomIn = new QToolButton(toolBar);
		btnEdgeZoomIn->setIcon(QIcon(":/images/zoomInEdge.png"));
		connect(btnEdgeZoomIn, SIGNAL(clicked()), _graph, SLOT(increaseEdgeSize()));

		QToolButton *btnEdgeZoomOut = new QToolButton(toolBar);
		btnEdgeZoomOut->setIcon(QIcon(":/images/zoomOutEdge.png"));
		connect(btnEdgeZoomOut, SIGNAL(clicked()), _graph, SLOT(decreaseEdgeSize()));

		QToolButton *btnMouseSelect = new QToolButton(toolBar);
		btnMouseSelect->setIcon(QIcon(":/images/mouseSelect.png"));
		connect(btnMouseSelect, SIGNAL(clicked()), _graph, SLOT(setMouseSelect()));

		QToolButton *btnCreateNode = new QToolButton(toolBar);
		btnCreateNode->setIcon(QIcon(":/images/createNode.png"));
		connect(btnCreateNode, SIGNAL(clicked()), _graph, SLOT(setMouseCreateNodes()));

		QToolButton *btnCreateEdge = new QToolButton(toolBar);
		btnCreateEdge->setIcon(QIcon(":/images/createEdge.png"));
		connect(btnCreateEdge, SIGNAL(clicked()), _graph, SLOT(setMouseCreateEdges()));


		QToolButton *resetConc = new QToolButton(toolBar);
		resetConc->setIcon(QIcon(":/images/reset.png"));
		connect(resetConc, SIGNAL(clicked()), _graph, SLOT(resetConcentrations()));
        connect(resetConc, SIGNAL(clicked()), this, SLOT(pause()));


		QToolButton *pause = new QToolButton(toolBar);
		pause->setIcon(QIcon(":/images/pause.png"));
		connect(pause, SIGNAL(clicked()), this, SLOT(pause()));

		QToolButton *go = new QToolButton(toolBar);
		go->setIcon(QIcon(":/images/go.png"));
		connect(go, SIGNAL(clicked()),this, SLOT(run()));

		QToolButton *save = new QToolButton(toolBar);
		save->setIcon(QIcon(":/images/save.png"));
		connect(save, SIGNAL(clicked()),this, SLOT(saveModelFile()));

		QToolButton *load = new QToolButton(toolBar);
		load->setIcon(QIcon(":/images/open.png"));
		connect(load, SIGNAL(clicked()),this, SLOT(loadModelFile()));



		toolBar->addWidget(load);
		toolBar->addWidget(save);

         toolBar->addWidget(go);
         toolBar->addWidget(pause);
         toolBar->addWidget(resetConc);

		toolBar->addWidget(btnMouseSelect);
		toolBar->addWidget(btnCreateNode);
		toolBar->addWidget(btnCreateEdge);

		toolBar->addWidget(btnZoomIn);
		toolBar->addWidget(btnZoomOut);
		toolBar->addWidget(btnNodeZoomIn);
		toolBar->addWidget(btnNodeZoomOut);
		toolBar->addWidget(btnEdgeZoomIn);
		toolBar->addWidget(btnEdgeZoomOut);
		toolBar->addWidget(btnTextZoomIn);
		toolBar->addWidget(btnTextZoomOut);


		setTitleBarWidget(toolBar);
}


void PathwayWidget::showGraph() {
	_graph->clear();
	_graph->newLayout();
	_graph->resetZoom();
	_graph->update();
	_graph->showLabels();

    }

void PathwayWidget::setAnimationSpeed(int newspeed) {
    if (timerId) killTimer(timerId);
    timerId = startTimer(newspeed);
}

void PathwayWidget::pause()  {   if (timerId) killTimer(timerId); timerId=0; qDebug() << "pause"; }
void PathwayWidget::run()    {   setAnimationSpeed(50);     qDebug() <<  "run"; }



void PathwayWidget::timerEvent(QTimerEvent*) {
    //updatePlots();
    _graph->scene()->advance();
    _graph->adjustNodeSizes();
}


void PathwayWidget::loadModelFile() {
    QStringList filelist = QFileDialog::getOpenFileNames(
            this,
            "Select Pathway File To Load",
            ".",
            "All Files(*.xml)");
     if ( filelist.size() == 0 || filelist[0].isEmpty() ) return;
	 _graph->clear();
	 _graph->loadModelFile(filelist[0]);
}

void PathwayWidget::saveModelFile() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save Pathway to a File"));
     if ( filename.isEmpty() ) return;
	 _graph->saveModelFile(filename);

}


