#ifndef OGOGRAPHVIEW_H
#define OGOGRAPHVIEW_H

#include <QtWidgets/QWidget>
#include <QColor>
#include <QTimer>
#include <QImage>
#include "../../../opengt.so/Headers/opengt.h"

class OGoGraphView : public QWidget
{
    static int colorsCount;
    static QColor colors[];

    Q_OBJECT
    int oldx;
    int oldy;

    OpenGraphtheory::Graph* graph;
    float zoom;
    float x_offset;
    float y_offset;
    float vertexsize;
    float edgewidth;
    float gridsize;
    int zoom_x;
    int zoom_y;
    std::string vertexcoloring;
    std::string edgecoloring;


    enum MouseAction {Nothing, Navigating, DrawingEdge, MovingVertex};
    MouseAction mouseaction;
    std::set<OpenGraphtheory::Graph::VertexIterator> selectedvertices;
    QString FileLocation;

    int WaitingForVertexSelection;
    std::vector<OpenGraphtheory::Graph::VertexIterator> PendingVertices;
    QWidget* VertexSelectionParent;
    void (*VertexSelectionCallback)(QWidget*, OGoGraphView*, std::vector<OpenGraphtheory::Graph::VertexIterator>, OpenGraphtheory::Graph*);

    QImage* Background;
    bool SnapToGrid;
    QTimer* Timer;
    float ZoomingTarget;

public:
    OGoGraphView(QWidget *parent = 0);
    ~OGoGraphView();

    void setGraph(OpenGraphtheory::Graph* graph);
    OpenGraphtheory::Graph* getGraph();
    void setBackground(QString FilePath);
    void setFileLocation(QString location);
    QString getFileLocation();

    void paintEvent(QPaintEvent *);
    void wheelEvent(QWheelEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void keyReleaseEvent(QKeyEvent *);

    OpenGraphtheory::Graph::VertexIterator VertexAt(int, int);
    void ScreenToModel(int, int, float&, float&);
    void ModelToScreen(float, float, int&, int&);

    void setVertexColoring(std::string coloringname);
    void setEdgeColoring(std::string coloringname);
    std::string getVertexColoring();
    std::string getEdgeColoring();
    void resetVertexColoring();
    void resetEdgeColoring();

    bool selectVertices(QWidget*, int, void callback(QWidget*, OGoGraphView*, std::vector<OpenGraphtheory::Graph::VertexIterator>, OpenGraphtheory::Graph*));

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
        void ZoomTimerTriggered();

};

#endif
