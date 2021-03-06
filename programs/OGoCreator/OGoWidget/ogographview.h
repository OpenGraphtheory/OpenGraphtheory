#ifndef OGOGRAPHVIEW_H
#define OGOGRAPHVIEW_H

#include <QtWidgets/QWidget>
#include <QColor>
#include <QTimer>
#include <QImage>
#include "../../../opengt.so/Headers/opengt.h"
#include "../../../opengt.so/Headers/visualize/renderer.h"

class OGoGraphView : public QWidget
{
    friend class OGoGraphRenderingContext;
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


    enum MouseAction {Nothing, Navigating, DrawingEdge, DrawingArc, MovingVertex};
    MouseAction mouseaction;
    std::vector<OpenGraphtheory::Vertex*> selectedvertices;
    QString FileLocation;

    int WaitingForVertexSelection;
    std::vector<OpenGraphtheory::Vertex*> PendingVertices;
    QWidget* VertexSelectionParent;
    void (*VertexSelectionCallback)(QWidget*, OGoGraphView*, std::vector<OpenGraphtheory::Vertex*>&, OpenGraphtheory::Graph*);

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

    OpenGraphtheory::VertexIterator VertexAt(int, int);
    void ScreenToModel(int, int, float&, float&);
    void ModelToScreen(float, float, int&, int&);

    void setVertexColoring(std::string coloringname);
    void setEdgeColoring(std::string coloringname);
    std::string getVertexColoring();
    std::string getEdgeColoring();
    void resetVertexColoring();
    void resetEdgeColoring();

    bool selectVertices(QWidget*, int, void callback(QWidget*, OGoGraphView*, std::vector<OpenGraphtheory::Vertex*>&, OpenGraphtheory::Graph*));

protected:
    void keyPressEvent(QKeyEvent *);

public slots:
        void ZoomTimerTriggered();

};

class OGoGraphRenderingContext : public OpenGraphtheory::Visualization::GraphRenderingContext
{
protected:
    OGoGraphView* target;
    QPainter* painter;
    QPen* pen;
    float LineWidth;
public:
    OGoGraphRenderingContext(OGoGraphView* target, QPainter* painter);
    ~OGoGraphRenderingContext();

    void SetPenColor(OpenGraphtheory::Visualization::Color color);
    void SetBrushColor(OpenGraphtheory::Visualization::Color color);
    void SetLineWidth(float width);

    void Line(float x1, float y1, float x2, float y2);
    void Circle(float x, float y, float radius);
};


#endif
