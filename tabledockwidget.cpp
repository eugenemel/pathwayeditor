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

    int columnCount=3;
    QStringList colNames; colNames << "id" << "Name";
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
    //QTreeWidgetItemIterator it(treeWidget);
    //while (*it) { updateItem(*it); ++it; }
    foreach( Node* n, node_item_map.keys()) updateItem(n);
}

void TableDockWidget::updateItem(Node* node) {
    if (node_item_map.contains(node)) {
        NumericTreeWidgetItem* item = this->node_item_map[node];
        item->setText(0,node->getId());
        item->setText(1,node->getNote());
    }
}


void TableDockWidget::removeItem(Node* node) {
    if (node_item_map.contains(node)) {
        NumericTreeWidgetItem* item = this->node_item_map[node];
        int idx = treeWidget->indexOfTopLevelItem(item);
        if(idx >= 0) {
            treeWidget->takeTopLevelItem(idx);
            node_item_map.remove(node);
            delete(item);
            qDebug() << "removeItem()";
        }
    }
}

void TableDockWidget::updateItem(QTreeWidgetItem* item) {
    heatmapBackground(item);
}

void TableDockWidget::heatmapBackground(QTreeWidgetItem* item) {
}

void TableDockWidget::addItem(Node* node) {
        qDebug() << "hello...signallled";
    addItem(node,0);
}

void TableDockWidget::addItem(Node* node, QTreeWidgetItem* root) {
    if (node == NULL) return;
    NumericTreeWidgetItem *item = new NumericTreeWidgetItem(root,0);
    item->setFlags(Qt::ItemIsSelectable |  Qt::ItemIsEnabled);;
    node_item_map[node] = item;
    if ( root == NULL ) treeWidget->addTopLevelItem(item);


    updateItem(node);
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
