#include <QMessageBox>
#include<QColor>
#include <cmath>
#include<QKeyEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QRect>
#include "ogographview.h"

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
    zoom = 1.0f;
    ZoomingTarget = 1.0f;
    x_offset = 0.0f;
    y_offset = 0.0f;

    vertexsize = 2.5f;
    edgewidth = 0.7f;
    oldx = 0;
    oldy = 0;
    gridsize = 5;
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

    QPainter p(this);
    p.fillRect(0,0,width(),height(), Qt::white);

    if(Background != NULL)
    {
        int bg_left, bg_width, bg_top, bg_height;
        ModelToScreen(0,0,bg_left,bg_top);
        ModelToScreen(Background->width(), Background->height(), bg_width, bg_height);

        p.drawImage(QRect(bg_left, bg_top, bg_width-bg_left, bg_height-bg_top), *Background);
    }

    // Draw Grid
    if(gridsize * zoom > 6)
    {
        p.setPen(Qt::lightGray);
        float increment = gridsize*zoom;

        for(float i = fmod(y_offset, increment); i < height(); i += increment)
            p.drawLine(0, i, width(), i);

        for(float i = fmod(x_offset, increment); i < width(); i += increment)
            p.drawLine(i, 0, i, height());
    }

    // Draw Edges
    QPen pen(Qt::black);
    pen.setWidthF(edgewidth*zoom);
    p.setPen(pen);
    for(Graph::EdgeIterator e = graph->BeginEdges(); e != graph->EndEdges(); e++)
    {
        vector<float> FromCoordinates = e.From().GetCoordinates();
        int fx, fy;
        ModelToScreen(FromCoordinates[0], FromCoordinates[1], fx, fy);

        vector<float> ToCoordinates = e.To().GetCoordinates();
        int tx, ty;
        ModelToScreen(ToCoordinates[0], ToCoordinates[1], tx, ty);

        pen.setColor(Qt::black);
        Attribute* attr = e.Attributes().GetAttribute(edgecoloring);
        if(attr != NULL)
        {
            IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
            if(iattr != NULL)
                pen.setColor(colors[iattr->Value % colorsCount]);
            BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
            if(battr != NULL && battr->Value)
                pen.setColor(colors[0]);
        }

        p.setPen(pen);
        p.drawLine(fx,fy,tx,ty);
    }

    // Draw Vertices
    for(Graph::VertexIterator v = graph->BeginVertices(); v != graph->EndVertices(); v++)
    {
        vector<float> Coordinates = v.GetCoordinates();
        int x, y;
        ModelToScreen(Coordinates[0] - vertexsize/2, Coordinates[1]-vertexsize/2, x, y);

        p.setPen(Qt::black);
        p.setBrush(Qt::black);
        Attribute* attr = v.Attributes().GetAttribute(vertexcoloring);
        if(attr != NULL)
        {
            IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
            if(iattr != NULL)
            {
                p.setPen(colors[iattr->Value % colorsCount]);
                p.setBrush(colors[iattr->Value % colorsCount]);
            }
            BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
            if(battr != NULL && battr->Value)
            {
                p.setPen(colors[0]);
                p.setBrush(colors[0]);
            }


        }
        p.drawEllipse(x, y, vertexsize*zoom, vertexsize*zoom);
    }

    if(mouseaction == OGoGraphView::DrawingEdge)
    {
        pen.setWidthF(edgewidth*zoom);
        p.setPen(pen);
        for(set<Graph::VertexIterator>::iterator i = selectedvertices.begin(); i != selectedvertices.end(); i++)
        {
            vector<float> Coordinates = i->GetCoordinates();
            int x, y;
            ModelToScreen(Coordinates[0], Coordinates[1], x, y);

            p.drawLine(x,y,oldx,oldy);
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
        mouseaction = mouseevent->modifiers() & Qt::ShiftModifier ? OGoGraphView::DrawingEdge : OGoGraphView::MovingVertex;
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
    if(mouseaction == OGoGraphView::DrawingEdge)
    {
        Graph::VertexIterator v = VertexAt(mouseevent->x(), mouseevent->y());
        if(v != graph->EndVertices())
        {
            for(set<Graph::VertexIterator>::iterator w = selectedvertices.begin(); w != selectedvertices.end(); w++)
                graph->AddEdge(v,*w);
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
            zoom = 1;
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






















