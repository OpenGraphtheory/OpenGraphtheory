#ifndef OGOGRAPHVIEW_H
#define OGOGRAPHVIEW_H

#include <QtGui/QWidget>
#include <QColor>
#include<QTimer>
#include <QImage>
#include "../../../OpenGraphtheory/opengt.so/Headers/opengt.h"

using namespace OpenGraphtheory;

class OGoGraphView : public QWidget
{
    static int colorsCount;
    static QColor colors[];

    Q_OBJECT
    int oldx;
    int oldy;

    Graph* graph;
    float zoom;
    float x_offset;
    float y_offset;
    float vertexsize;
    float edgewidth;
    float gridsize;
    std::string vertexcoloring;
    std::string edgecoloring;


    enum MouseAction {Nothing, Navigating, DrawingEdge, MovingVertex};
    MouseAction mouseaction;
    std::set<Graph::VertexIterator> selectedvertices;
    QString FileLocation;

    int WaitingForVertexSelection;
    std::vector<Graph::VertexIterator> PendingVertices;
    QWidget* VertexSelectionParent;
    void (*VertexSelectionCallback)(QWidget*, OGoGraphView*, vector<Graph::VertexIterator>, Graph*);

    QImage* Background;
    bool SnapToGrid;
    QTimer* Timer;
    float ZoomingTarget;

public:
    OGoGraphView(QWidget *parent = 0);
    ~OGoGraphView();

    void setGraph(Graph* graph);
    Graph* getGraph();
    void setBackground(QString FilePath);
    void setFileLocation(QString location);
    QString getFileLocation();

    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void keyReleaseEvent(QKeyEvent *);

    Graph::VertexIterator VertexAt(int, int);
    void ScreenToModel(int, int, float&, float&);
    void ModelToScreen(float, float, int&, int&);

    void setVertexColoring(std::string coloringname);
    void setEdgeColoring(std::string coloringname);
    void resetVertexColoring();
    void resetEdgeColoring();

    bool selectVertices(QWidget*, int, void callback(QWidget*, OGoGraphView*, vector<Graph::VertexIterator>, Graph*));

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
        void ZoomTimerTriggered();

};

#endif
