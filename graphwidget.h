#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include "stable.h"
#include "node.h"
#include "edge.h"
#include <string>

#ifdef LPSOLVE
#include "lp_lib.h"
#endif

using namespace std;

class Node;
class Edge;

class MyScene : public QGraphicsScene
{
    Q_OBJECT

public:
    MyScene(QObject * parent = 0): QGraphicsScene(parent) {
        _arrow=NULL;
    }

    QPointF getLastCursorPos() { return lastCursorPos; }
    QPointF getLastMousePressPos()   { return down; }
    QPointF getLastMouseReleasePos() { return up; }
    void showArrow(bool flag) { _showArrow=flag; }

	protected:
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent );
    void mouseMoveEvent ( QGraphicsSceneMouseEvent * mouseEvent );

	signals:
    void zoomArea(QRectF);
    void mousePressed();
    void mouseReleased();

	private:
    QPointF down;
    QPointF up;
    QPointF lastCursorPos;

    bool _showArrow;
    QGraphicsLineItem* _arrow;

};


class GraphWidget : public QGraphicsView
{
    Q_OBJECT
    friend class PathwayWidget;

public:
    GraphWidget();
    ~GraphWidget();

    void clear();
    void itemMoved();
    Edge* addEdge(Node* a, Node* b,QString id, void* data=0);
    Node* addNode(QString id, void* data=0);
    Node* locateNode(QString id);
    void removeNode(Node* n);
    void removeEdge(Edge* e);
    Edge* findEdge(Node* n1, Node* n2);

    void  setNodeSizeScale( float scale) { _nodeSizeScale=scale; }
    float getNodeSizeScale() { return _nodeSizeScale; }

    void  setLabelSizeScale( float scale) { _labelSizeScale=scale; }
    float getLabelSizeScale() { return _labelSizeScale; }

    void  setEdgeSizeScale( float scale) { _edgeSizeScale=scale; }
    float getEdgeSizeScale() { return _edgeSizeScale; }

    enum  sizeNormalization { FixedSize=1, AbsoluteSize=2, RelativeSize=3, PairwiseSize=4 };
    sizeNormalization getNodeSizeNormalization() { return _nodeSizeNormalization; }

    enum  LayoutAlgorithm { FMMM=1, Circular=2, Balloon=3, Random=4 };
    LayoutAlgorithm getLayoutAlgorithm() { return _layoutAlgorithm; }

    enum  MouseState { mouseSelect=0, mouseCreateNode=1, mouseCreateEdge=2 };
    MouseState getMouseState() { return _mouseState; }

    void  setTitle(const QString& title);
    void  showLabels();
    void  makeFBAModel();
    void  runFBA();


    QList<Node*> getNodes(int type);

public slots:
    void resetZoom();
    void zoomIn() { scale(1.2, 1.2); }
    void zoomOut() { scale(1 / 1.2, 1 / 1.2); }
    void zoomArea(QRectF sceneArea) { fitInView(sceneArea,Qt::KeepAspectRatio); }
    void increaseLabelSize() { setLabelSizeScale(getLabelSizeScale()*1.1); showLabels(); scene()->update(); }
    void decreaseLabelSize() { setLabelSizeScale(getLabelSizeScale()*0.9); showLabels(); scene()->update(); }
    void increaseNodeSize() {  setNodeSizeScale(getNodeSizeScale()*1.1);   scene()->update(); }
    void decreaseNodeSize() {  setNodeSizeScale(getNodeSizeScale()*0.9);   scene()->update(); }
    void increaseEdgeSize() {  setEdgeSizeScale(getEdgeSizeScale()*1.1);   scene()->update(); }
    void decreaseEdgeSize() {  setEdgeSizeScale(getEdgeSizeScale()*0.9);   scene()->update(); }
    void setMouseSelect() 	   { _mouseState = mouseSelect; setCursor(Qt::ArrowCursor); }
    void setMouseCreateNodes() { _mouseState = mouseCreateNode; setCursor(Qt::PointingHandCursor); }
    void setMouseCreateEdges() { _mouseState = mouseCreateEdge; setCursor(Qt::CrossCursor);}
    void mouseEventProcessing();
    void resetConcentrations();
	void saveModelFile(QString filename);
	void loadModelFile(QString filename);

    void createNode();
    void createEdge();
    void addToSelectionQ(Node* node);
    void clearSelectionQ() { _selectionQ.clear(); }

    void computeAvgEdgeLength();
    float getAvgEdgeLength() { return _averageEdgeSize; }

    void  setLayoutAlgorithm(LayoutAlgorithm x) { _layoutAlgorithm = x; }
    void  setNodeSizeNormalization(sizeNormalization x) { _nodeSizeNormalization = x; }


    void dump();
    void layoutOGDF();
    void updateLayout();
    void newLayout();
    void hideLongEdges();
    void hideEdges();
    void adjustLayout();
    void adjustLayout(Node*);
    void adjustEnzymePositions();
    void clearLayout();
    void removeSelectedNodes();
    void addToTree(Node* a, Node* b);

    void setBackgroundImage(QString filename);
    QString getBackgroundImageFilename() { return _backgroundImageFile; }

signals:
    void titleChanged(QString);

protected:
    void updateSceneRect();
    void adjustNodeSizes();
    void drawBackground(QPainter *painter, const QRectF &rect);
    void keyPressEvent(QKeyEvent *event);
    //void resizeEvent( QResizeEvent * ) { resetZoom(); }

    void scaleView(qreal scaleFactor);
    void randomNodePositions();
    void recursiveDepth(Node* n0,int depth);
    void recursiveDraw(QTreeWidgetItem* item);
    QPointer<QGraphicsTextItem> _title;

    QHash<QString,Node*> nodelist;
    QHash<QString,Edge*> edgelist;

    float len(float b1, float b2) { return sqrt(b1*b1 + b2*b2); }
    float attraction (float d, float k) { return d*d/k; }
    float repulsion (float d, float k)  { return k*k/d; }
    float cool (float temp, float initial_temp, int rep_max) { temp -= initial_temp / rep_max; return temp < 0 ? 0 : temp; }


private:
    //graph layout
    
    MyScene* _myscene;
    QVector<Node*> _selectionQ;

    sizeNormalization _nodeSizeNormalization;
    LayoutAlgorithm	  _layoutAlgorithm;
    MouseState        _mouseState;

    QTreeWidget* layoutTree;
    QHash<Node*, QTreeWidgetItem*>layoutMap;
    void deepChildCount(QTreeWidgetItem* x, int* count);

    float _nodeSizeScale;
    float _labelSizeScale;
    float _edgeSizeScale;
    float _averageEdgeSize;
    bool  _updateLayout;

    int   _uniqEdgeCounter;
    int   _uniqNodeCounter;

    QString _backgroundImageFile;
    QPixmap _backgroundImage;

};

#endif
