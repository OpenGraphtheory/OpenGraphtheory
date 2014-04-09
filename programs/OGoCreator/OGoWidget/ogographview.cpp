#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QColor>
#include <cmath>
#include <QKeyEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QRect>
#include "ogographview.h"

using namespace std;
using namespace OpenGraphtheory;

int OGoGraphView::colorsCount = 8;
QColor OGoGraphView::colors[] = {
    QColor(0xE5,0x00,0x00), // red
    QColor(0xF3,0xD5,0x10), // yellow
    QColor(0x34,0xCD,0x20), // green
    QColor(0x1C,0x23,0xEB), // dark blue
    QColor(0xF3,0x7D,0x23), // orange
    QColor(0x15,0xCD,0xC7), // light blue
    QColor(0x87,0x22,0xE5), // purple
    QColor(0xFF,0x33,0xFF), // pink
};

OGoGraphView::OGoGraphView(QWidget *parent) :
    QWidget(parent)
{    
    // set the zoom to the resolution of the monitor in dots-per-centimeter
    // (division by 2.54 is the conversion from DPI to DPCM)
    zoom = QApplication::desktop()->physicalDpiX() / 2.54;
    ZoomingTarget = zoom;
    x_offset = 0.0f;
    y_offset = 0.0f;

    vertexsize = 2.5f;
    edgewidth = 0.7f;
    oldx = 0;
    oldy = 0;
    gridsize = 1.0f;
    Background = NULL;
    SnapToGrid = true;

    VertexSelectionCallback = NULL;
    VertexSelectionParent = NULL;
    WaitingForVertexSelection = -1;
    Timer = new QTimer(this);
    Timer->setSingleShot(false);
    Timer->setInterval(50);
    connect(Timer, SIGNAL(timeout()), this, SLOT(ZoomTimerTriggered()));

    setFocusPolicy(Qt::WheelFocus);
    setMouseTracking(true);
}

void OGoGraphView::wheelEvent(QWheelEvent *wheelevent)
{
    if(wheelevent->delta() > 0)
        ZoomingTarget *= 2;
    if(wheelevent->delta() < 0)
        ZoomingTarget /= 2;
    zoom_x = oldx;
    zoom_y = oldy;

    Timer->start();
}


void OGoGraphView::ZoomTimerTriggered()
{
    float f_oldx, f_oldy;
    ScreenToModel(zoom_x,zoom_y,f_oldx, f_oldy);

    if(abs(ZoomingTarget-zoom) < 0.01f)
    {
        zoom = ZoomingTarget;
        Timer->stop();
    }
    else
    {
        zoom += (ZoomingTarget-zoom) * 0.29f;
    }

    int newx, newy;
    ModelToScreen(f_oldx,f_oldy,newx,newy);
    // and move the screen there
    x_offset -= (newx - zoom_x);
    y_offset -= (newy - zoom_y);

    repaint();
}

OGoGraphView::~OGoGraphView()
{
    if(Background != NULL)
        delete Background;
    delete graph;
}

void OGoGraphView::ScreenToModel(int sx, int sy, float& mx, float& my)
{
    mx = (sx-x_offset) / zoom;
    my = (sy-y_offset) / zoom;
}

void OGoGraphView::ModelToScreen(float mx, float my, int &sx, int &sy)
{
    sx = mx*zoom + x_offset;
    sy = my*zoom + y_offset;
}

void OGoGraphView::setGraph(Graph* graph)
{
    this->graph = graph;
}

Graph* OGoGraphView::getGraph()
{
    return graph;
}

void OGoGraphView::setFileLocation(QString location)
{
    this->FileLocation = location;
}

QString OGoGraphView::getFileLocation()
{
    return FileLocation;
}


std::string OGoGraphView::getEdgeColoring()
{
    return edgecoloring;
}

std::string OGoGraphView::getVertexColoring()
{
    return vertexcoloring;
}

void OGoGraphView::setEdgeColoring(std::string coloring)
{
    this -> edgecoloring = coloring;
    repaint();
}

void OGoGraphView::setVertexColoring(std::string coloring)
{
    this -> vertexcoloring = coloring;
    repaint();
}

void OGoGraphView::resetEdgeColoring()
{
    this -> edgecoloring = "";
    repaint();
}

void OGoGraphView::resetVertexColoring()
{
    this -> vertexcoloring = "";
    repaint();
}

void OGoGraphView::setBackground(QString FilePath)
{
    if(Background != NULL)
        delete Background;
    Background = new QImage(FilePath);
    repaint();
}

void OGoGraphView::paintEvent(QPaintEvent *)
{
    if(graph == NULL)
        return;

    QPainter painter(this);
    painter.fillRect(0,0,width(),height(), Qt::white);

    if(Background != NULL)
    {
        int bg_left, bg_width, bg_top, bg_height;
        ModelToScreen(0,0,bg_left,bg_top);
        ModelToScreen(Background->width(), Background->height(), bg_width, bg_height);

        painter.drawImage(QRect(bg_left, bg_top, bg_width-bg_left, bg_height-bg_top), *Background);
    }

    // Draw Grid
    if(gridsize * zoom > 6)
    {
        painter.setPen(Qt::lightGray);
        float increment = gridsize*zoom;

        for(float i = fmod(y_offset, increment); i < height(); i += increment)
            painter.drawLine(0, i, width(), i);

        for(float i = fmod(x_offset, increment); i < width(); i += increment)
            painter.drawLine(i, 0, i, height());
    }

    QPen pen(Qt::darkGray);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(0, y_offset, width(), y_offset);
    painter.drawLine(x_offset, 0, x_offset, height());



    OGoGraphRenderingContext* context = new OGoGraphRenderingContext(this, &painter);
    context->RenderGraph(*graph, vertexcoloring, edgecoloring, 74, -1, -1);
    delete context;


    pen.setColor(Qt::black);
    if(mouseaction == OGoGraphView::DrawingEdge || mouseaction == OGoGraphView::DrawingArc)
    {
        pen.setWidthF(edgewidth*zoom);
        painter.setPen(pen);
        for(set<Graph::VertexIterator>::iterator i = selectedvertices.begin(); i != selectedvertices.end(); i++)
        {
            vector<float> Coordinates = i->GetCoordinates();
            int x, y;
            ModelToScreen(Coordinates[0], Coordinates[1], x, y);

            painter.drawLine(x,y,oldx,oldy);
        }
    }
}

Graph::VertexIterator OGoGraphView::VertexAt(int x, int y)
{
    float mx, my;
    ScreenToModel(x,y,mx,my);
    for(Graph::VertexIterator v = graph->BeginVertices(); v != graph->EndVertices(); v++)
    {
        vector<float> coordinates = v.GetCoordinates();
        float vx = coordinates[0];
        float vy = coordinates[1];
        if(mx > vx - vertexsize/2 && mx < vx + vertexsize/2
           && my > vy - vertexsize/2 && my < vy + vertexsize/2)
        {
            if( (mx-vx)*(mx-vx) + (my-vy)*(my-vy) < (vertexsize*vertexsize/4))
                return v;
        }
    }

    return graph->EndVertices();
}


bool OGoGraphView::selectVertices(QWidget* parent, int count, void callback(QWidget*, OGoGraphView*, vector<Graph::VertexIterator>, Graph*))
{
    if(WaitingForVertexSelection > 0)
        return false;
    WaitingForVertexSelection = count;
    VertexSelectionCallback = callback;
    VertexSelectionParent = parent;
    PendingVertices.clear();
    return true;
}

void OGoGraphView::mousePressEvent(QMouseEvent *mouseevent)
{
    int x = mouseevent->x();
    int y = mouseevent->y();

    if(WaitingForVertexSelection > 0)
    {
        Graph::VertexIterator v = VertexAt(x,y);
        if(v != graph->EndVertices())
        {
            PendingVertices.push_back(v);
            WaitingForVertexSelection--;
        }

        if(WaitingForVertexSelection <= 0)
        {
            WaitingForVertexSelection = -1;
            VertexSelectionCallback(VertexSelectionParent, this, PendingVertices, graph);
        }

        return;
    }

    mouseaction = OGoGraphView::Nothing;
    Graph::VertexIterator v = VertexAt(x,y);
    if(v != graph->EndVertices())
    {
        if((mouseevent->modifiers() & Qt::ShiftModifier) && (mouseevent->modifiers() & Qt::AltModifier))
        {
            mouseaction = OGoGraphView::DrawingArc;
        }
        else if(mouseevent->modifiers() & Qt::ShiftModifier)
        {
            mouseaction = OGoGraphView::DrawingEdge;
        }
        else
        {
            mouseaction = OGoGraphView::MovingVertex;
        }

        if(!(mouseevent->modifiers() & Qt::ControlModifier))
            selectedvertices.clear();
        selectedvertices.insert(v);
    }

    if(mouseaction == OGoGraphView::Nothing)
        mouseaction = OGoGraphView::Navigating;

    oldx = x;
    oldy = y;
}

void OGoGraphView::mouseReleaseEvent(QMouseEvent *mouseevent)
{
    if(mouseaction == OGoGraphView::DrawingEdge || mouseaction == OGoGraphView::DrawingArc)
    {
        Graph::VertexIterator v = VertexAt(mouseevent->x(), mouseevent->y());
        if(v != graph->EndVertices())
        {
            for(set<Graph::VertexIterator>::iterator w = selectedvertices.begin(); w != selectedvertices.end(); w++)
            {
                if(mouseaction == OGoGraphView::DrawingEdge)
                    graph->AddEdge(*w,v);
                else
                    graph->AddArc(*w,v);
            }
        }
    }

    mouseaction = OGoGraphView::Nothing;

    oldx = mouseevent->x();
    oldy = mouseevent->y();
    repaint();
}

void OGoGraphView::mouseMoveEvent(QMouseEvent *mouseevent)
{
    Graph::VertexIterator v = VertexAt(mouseevent->x(), mouseevent->y());
    if(v != graph->EndVertices())
        this -> setToolTip(v.GetLabel().c_str());

    switch(mouseaction)
    {
        case OGoGraphView::Navigating:
        {
            x_offset += mouseevent->x() - oldx;
            y_offset += mouseevent->y() - oldy;
            repaint();
            break;
        }

        case OGoGraphView::MovingVertex:
        {
            for(set<Graph::VertexIterator>::iterator v = selectedvertices.begin(); v != selectedvertices.end(); v++)
            {
                if(SnapToGrid)
                {
                    vector<float> coordinates = v->GetCoordinates();
                    float modelmousex, modelmousey;
                    ScreenToModel(mouseevent->x(), mouseevent->y(), modelmousex, modelmousey);

                    coordinates[0] = floor(modelmousex/gridsize + 0.5) * gridsize;
                    coordinates[1] = floor(modelmousey/gridsize + 0.5) * gridsize;
                    Graph::VertexIterator vi = *v;
                    vi.SetCoordinates(coordinates);
                }
                else
                {
                    vector<float> coordinates = v->GetCoordinates();
                    coordinates[0] += (mouseevent->x()-oldx)/zoom;
                    coordinates[1] += (mouseevent->y()-oldy)/zoom;
                    Graph::VertexIterator vi = *v;
                    vi.SetCoordinates(coordinates);
                }
            }
            repaint();
            break;
        }

        case OGoGraphView::DrawingEdge:
        case OGoGraphView::DrawingArc:
        {
            repaint();
            break;
        }

        case OGoGraphView::Nothing:
            break;
    }

    oldx = mouseevent->x();
    oldy = mouseevent->y();
}

void OGoGraphView::keyPressEvent(QKeyEvent *keyevent)
{
    switch(keyevent->key())
    {
        case Qt::Key_Delete:
        {
            for(set<Graph::VertexIterator>::iterator i = selectedvertices.begin(); i != selectedvertices.end(); i++)
                graph->RemoveVertex(*i);
            selectedvertices.clear();
            repaint();
            break;
        }

        case Qt::Key_Insert:
        {
            float mx, my;
            ScreenToModel(oldx, oldy, mx, my);
            if(SnapToGrid)
            {
                mx = floor(mx/gridsize + 0.5) * gridsize;
                my = floor(my/gridsize + 0.5) * gridsize;
            }

            graph->AddVertex(mx, my);
            repaint();
            break;
        }

        case Qt::Key_Escape:
        {
            x_offset = 0;
            y_offset = 0;
            zoom = QApplication::desktop()->physicalDpiX() / 2.54;
            repaint();
            break;
        }

        default:
        {
            QWidget::keyReleaseEvent(keyevent);
            break;
        }
    }
}

void OGoGraphView::keyReleaseEvent(QKeyEvent*)
{

}




OGoGraphRenderingContext::OGoGraphRenderingContext(OGoGraphView* target, QPainter* painter)
{
    this->target = target;
    this->painter = painter;
    this->pen = new QPen();
    painter->setPen(*pen);
}

OGoGraphRenderingContext::~OGoGraphRenderingContext()
{

}

void OGoGraphRenderingContext::SetPenColor(OpenGraphtheory::Visualization::Color color)
{
    pen->setColor(QColor(color.Red, color.Green, color.Blue));
    painter->setPen(*pen);
}

void OGoGraphRenderingContext::SetBrushColor(OpenGraphtheory::Visualization::Color color)
{
    painter->setBrush(QColor(color.Red, color.Green, color.Blue));
}

void OGoGraphRenderingContext::SetLineWidth(float width)
{
    pen->setWidthF(width*target->zoom);
    painter->setPen(*pen);
}

void OGoGraphRenderingContext::Line(float x1, float y1, float x2, float y2)
{
    x1 += OffsetX;
    y1 += OffsetY;
    x2 += OffsetX;
    y2 += OffsetY;
    int ix1, iy1, ix2, iy2;
    this->target->ModelToScreen(x1,y1, ix1, iy1);
    this->target->ModelToScreen(x2,y2, ix2, iy2);

    painter->drawLine(ix1,iy1, ix2, iy2);
}

void OGoGraphRenderingContext::Circle(float x, float y, float radius)
{
    x += OffsetX;
    y += OffsetY;
    int ix, iy;
    target->ModelToScreen(x,y,ix,iy);

    painter->drawEllipse(ix-radius*target->zoom, iy-radius*target->zoom, 2*radius*target->zoom, 2*radius*target->zoom);
}
















