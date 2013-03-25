#include "tabledockwidget.h";

TableDockWidget::TableDockWidget(MainWindow* mw) {
    setAllowedAreas(Qt::AllDockWidgetAreas);
    setFloating(false);
    _mainwindow = mw;

    viewType = nodeView; 

    treeWidget=new QTreeWidget(this);
    treeWidget->setSortingEnabled(false);
    treeWidget->setColumnCount(2);
    treeWidget->setDragDropMode(QAbstractItemView::DragOnly);
    treeWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
    treeWidget->setAcceptDrops(false);    
    treeWidget->setObjectName("Spreadsheet");
    this->setAcceptDrops(true);

    setWidget(treeWidget);
    setupTable();

    QToolBar *toolBar = new QToolBar(this);
    toolBar->setFloatable(false);
    toolBar->setMovable(false);

    QToolButton *btnSwitchView = new QToolButton(toolBar);
    btnSwitchView->setIcon(QIcon(":images/flip.png"));
    btnSwitchView->setToolTip("Switch between Group and Peak Views");
    connect(btnSwitchView,SIGNAL(clicked()),SLOT(switchTableView()));

    QToolButton *btnLoad = new QToolButton(toolBar);
    btnLoad->setIcon(QIcon(":images/fileopen.png"));
    btnLoad->setToolTip("Load Peaks");
    connect(btnLoad, SIGNAL(clicked()), SLOT(loadPeakTable()));

    toolBar->addWidget(btnSwitchView);
    toolBar->addWidget(btnLoad);
    setTitleBarWidget(toolBar);
    setAcceptDrops(true);
}

TableDockWidget::~TableDockWidget() { 

}

void TableDockWidget::setupTable() {

    int columnCount=2;
    QStringList colNames;
    for(unsigned int i=0; i<columnCount; i++ ) {
          colNames << QString::number(i);
    }

    treeWidget->setColumnCount(colNames.size());
    treeWidget->setHeaderLabels(colNames);
    treeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
    treeWidget->header()->adjustSize();
    treeWidget->setSortingEnabled(true);
}

void TableDockWidget::updateTable() {
    QTreeWidgetItemIterator it(treeWidget);
    while (*it) {
        updateItem(*it);
        ++it;
    }
}

void TableDockWidget::updateItem(QTreeWidgetItem* item) {
    heatmapBackground(item);
}

void TableDockWidget::heatmapBackground(QTreeWidgetItem* item) {
}


void TableDockWidget::addNode(Node* group, QTreeWidgetItem* root) { 

    if (group == NULL) return;
    NumericTreeWidgetItem *item = new NumericTreeWidgetItem(root,0);
    item->setFlags(Qt::ItemIsSelectable |  Qt::ItemIsEnabled | Qt::ItemIsDragEnabled);

    if (viewType == nodeView) {
   /*     item->setText(3,QString::number(group->sampleCount));
        item->setText(4,QString::number(group->goodPeakCount));
        item->setText(5,QString::number(group->maxNoNoiseObs));
        item->setText(6,QString::number(group->maxIntensity,'g',2));
        item->setText(7,QString::number(group->maxSignalBaselineRatio,'f',0));
        item->setText(8,QString::number(group->maxQuality,'f',2));

        if ( group->changeFoldRatio != 0 ) {
            item->setText(9,QString::number(group->changeFoldRatio, 'f', 2));
            item->setText(10,QString::number(group->changePValue,    'e', 4));
        }
   */  
   }
    if ( root == NULL ) treeWidget->addTopLevelItem(item);
    updateItem(item);
}


void TableDockWidget::keyPressEvent(QKeyEvent *e ) {
    //cerr << "TableDockWidget::keyPressEvent()" << e->key() << endl;

    QTreeWidgetItem *item = treeWidget->currentItem();
    if (e->key() == Qt::Key_Delete ) {
    //    deleteGroup();
    }
    QDockWidget::keyPressEvent(e);
}

void TableDockWidget::contextMenuEvent ( QContextMenuEvent * event ) 
{
    QMenu menu;
}

void TableDockWidget::loadPeakTable() {
    QString dir = ".";
    QString selFilter;
    QStringList filters;
    filters << "Text Delimited(*.tab *.csv *.txt *.tsv)";

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Load Saved Peaks",
                                                    dir,
                                                    filters.join(";;"),
                                                    &selFilter);
    if (fileName.isEmpty()) return;
    loadCSVFile(fileName,"\t");
}


void TableDockWidget::dragEnterEvent(QDragEnterEvent *event)
{
    foreach (QUrl url, event->mimeData()->urls() ) {
        std::cerr << "dragEnterEvent:" << url.toString().toStdString() << endl;
        if (url.toString() == "ok") {
            event->acceptProposedAction();
            return;
        } else {
            return;
        }
    }
}

void TableDockWidget::dropEvent(QDropEvent *event)
 {
    foreach (QUrl url, event->mimeData()->urls() ) {
         std::cerr << "dropEvent:" << url.toString().toStdString() << endl;
    }
 }



int TableDockWidget::loadSpreadsheet(QString fileName){
     qDebug() << "Loading SpreadSheet   : " << fileName;

     if( fileName.endsWith(".txt",Qt::CaseInsensitive)) {
       loadCSVFile(fileName,"\t");
    } else if( fileName.endsWith(".csv",Qt::CaseInsensitive)) {
        loadCSVFile(fileName,",");
    } else if( fileName.endsWith(".tsv",Qt::CaseInsensitive)) {
        loadCSVFile(fileName,"\t");
    } else if( fileName.endsWith(".tab",Qt::CaseInsensitive)) {
        loadCSVFile(fileName,"\t");
    }
}

int TableDockWidget::loadCSVFile(QString filename, QString sep="\t"){

    if(filename.isEmpty()) return 0;

    QFile myfile(filename);
    if(!myfile.open(QIODevice::ReadOnly | QIODevice::Text)) return 0;

    QTextStream stream(&myfile);
    if (stream.atEnd()) return 0;

    QString line;
    int lineCount=0;
    QMap<QString, int>headerMap;
    QStringList header;

    do {
         line = stream.readLine();
         if (line.isEmpty() || line[0] == '#') continue;
         QStringList fields = line.split(sep);
        lineCount++;
         if (lineCount==1) { //header line
             for(int i=0; i < fields.size(); i++ ) {
                 fields[i] = fields[i].toLower();
                 fields[i].replace("\"","");
                 headerMap[ fields[i] ] = i;
                 header << fields[i];
             }
             qDebug() << header  << endl;
         } else {
         }
     } while (!line.isNull());

    return lineCount;
}


void TableDockWidget::switchTableView() {
    viewType == nodeView ? viewType=edgeView: viewType=nodeView;
    setupTable();
    updateTable();
}
