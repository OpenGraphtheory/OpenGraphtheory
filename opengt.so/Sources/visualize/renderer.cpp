
#include "../../Headers/visualize/renderer.h"

using namespace std;
using namespace OpenGraphtheory::Export;

namespace OpenGraphtheory
{
    namespace Visualization
    {
        GraphicsContext::GraphicsContext()
        {
        }

        GraphicsContext::~GraphicsContext()
        {
        }

        void GraphicsContext::SetPenColor(Color)
        {
        }

        void GraphicsContext::SetBrushColor(Color)
        {
        }

        void GraphicsContext::SetLineWidth(float Width)
        {
        }

        void GraphicsContext::Line(float x1, float y1, float x2, float y2)
        {

        }

        void GraphicsContext::Arrow(float x1, float y1, float x2, float y2)
        {
            float ArrowHeadAngle = 40.0f / 2.0f;
            float ArrowHeadLength = 3;

            Visualization::Vector2D temp(x2-x1, y2-y1);
            float alpha = temp.Theta();

            float arrowX1 = x2-ArrowHeadLength*cos((alpha-ArrowHeadAngle)*M_PI/180.0);
            float arrowY1 = y2-ArrowHeadLength*sin((alpha-ArrowHeadAngle)*M_PI/180.0);
            float arrowX2 = x2-ArrowHeadLength*sin((90-alpha-ArrowHeadAngle)*M_PI/180.0);
            float arrowY2 = y2-ArrowHeadLength*cos((90-alpha-ArrowHeadAngle)*M_PI/180.0);

            Line(x1, y1, x2, y2);
            // arrow head
            Line(arrowX1, arrowY1, x2, y2);
            Line(arrowX2, arrowY2, x2, y2);
            Line(arrowX1, arrowY1, arrowX2, arrowY2);
        }

        void GraphicsContext::Circle(float x, float y, float radius)
        {
        }

        void GraphicsContext::PutText(float x, float y, std::string text)
        {
        }

        // ==============================================================================================================

        GraphRenderingContext::GraphRenderingContext()
            : GraphicsContext(),
            LastPenColor(0,0,0),
            LastBrushColor(0,0,0)
        {
            OffsetX = 0;
            OffsetY = 0;
            LastLineWidth = 1;
            Translator = NULL;
        }

        GraphRenderingContext::~GraphRenderingContext()
        {
            if(Translator != NULL)
                delete Translator;
        }

        void GraphRenderingContext::RenderGraph(Graph& G, string vertexcoloring, string edgecoloring, float dpi,
                         float edgewidth, float vertexradius)
        {
            map<Graph::VertexIterator, int> vcoloringInt = G.GetVertexColoring(vertexcoloring);
            map<Graph::VertexIterator, Color> VertexColoring;
            for(map<Graph::VertexIterator, int>::iterator i = vcoloringInt.begin(); i != vcoloringInt.end(); i++)
                VertexColoring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];

            map<Graph::EdgeIterator, Color> EdgeColoring;
            map<Graph::EdgeIterator, int> ecoloringInt = G.GetEdgeColoring(edgecoloring);
            for(map<Graph::EdgeIterator, int>::iterator i = ecoloringInt.begin(); i != ecoloringInt.end(); i++)
                EdgeColoring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];

            RenderGraph(G, VertexColoring, EdgeColoring, dpi, edgewidth, vertexradius);
        }

        void GraphRenderingContext::RenderGraph(Graph& G, std::map<Graph::VertexIterator, Color>& vertexcoloring,
                         std::map<Graph::EdgeIterator, Color>& edgecoloring, float dpi,
                         float edgewidth, float vertexradius)
        {
            Graph::VertexIterator v1 = G.BeginVertices();
            vector<float> coordinates = v1.GetCoordinates();
            float minx = coordinates[0], maxx = coordinates[1], maxy = coordinates[1], miny = coordinates[1];
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                coordinates = v1.GetCoordinates();
                float radius = vertexradius >= 0 ? vertexradius : v1.GetWeight();

                if(coordinates[0]-radius < minx)
                    minx = coordinates[0]-radius;
                if(coordinates[1]-radius < miny)
                    miny = coordinates[1]-radius;
                if(coordinates[0]+radius > maxx)
                    maxx = coordinates[0]+radius;
                if(coordinates[1]+radius > maxy)
                    maxy = coordinates[1]+radius;
            }


            this->OffsetX = minx;
            this->OffsetY = miny;
            this->BeginRenderingGraph(maxx-minx, maxy-miny, dpi);


            // Declare Vertices (if necessary)
            this->BeginDeclaringVertices();
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();
                Color color = vertexcoloring.find(v) != vertexcoloring.end() ? vertexcoloring[v] : Color(0,0,0);
                float radius = vertexradius >= 0 ? vertexradius : v.GetWeight();
                string label = Translator != NULL ? Translator->Translate(v.GetLabel()) : v.GetLabel();
                this->DeclareVertex(v.GetID(),
                                    coordinates[0] - minx,
                                    coordinates[1] - miny,
                                    radius,
                                    label,
                                    color);
            }
            this->EndDeclaringVertices();


            // Render Edges
            this->BeginRenderingEdges();
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                vector<float> FromCoordinates = e.From().GetCoordinates();
                vector<float> ToCoordinates = e.To().GetCoordinates();
                float x1 = FromCoordinates[0] - minx;
                float y1 = FromCoordinates[1] - miny;
                float x2 = ToCoordinates[0] - minx;
                float y2 = ToCoordinates[1] - miny;

                Color color = edgecoloring.find(e) != edgecoloring.end() ? edgecoloring[e] : Color(0,0,0);
                float LineWidth = edgewidth >= 0.0f ? edgewidth : e.GetWeight();
                string label = Translator != NULL ? Translator->Translate(e.GetLabel()) : e.GetLabel();

                if(e.IsEdge())
                {
                    this->RenderEdge(e.From().GetID(),
                                     e.To().GetID(),
                                     x1,
                                     y1,
                                     x2,
                                     y2,
                                     LineWidth,
                                     label,
                                     color);
                }
                else
                {
                    float TargetRadius = vertexradius >= 0 ? vertexradius : e.To().GetWeight();
                    Visualization::Vector2D intersectionpoint(x2-x1, y2-y1);
                    intersectionpoint = intersectionpoint * ((intersectionpoint.Length()-TargetRadius) / intersectionpoint.Length());
                    intersectionpoint.x += x1;
                    intersectionpoint.y += y1;
                    this->RenderArc(e.From().GetID(),
                                    e.To().GetID(),
                                    x1,
                                    y1,
                                    intersectionpoint.x,
                                    intersectionpoint.y,
                                    LineWidth,
                                    label,
                                    color);
                }
            }
            this->EndRenderingEdges();


            // Render Vertices
            this->BeginRenderingVertices();
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();
                float Radius = vertexradius >= 0 ? vertexradius : v.GetWeight();
                Color color = vertexcoloring.find(v) != vertexcoloring.end() ? vertexcoloring[v] : Color(0,0,0);
                string label = Translator != NULL ? Translator->Translate(v.GetLabel()) : v.GetLabel();

                this->RenderVertex(v.GetID(),
                                   coordinates[0] - minx,
                                   coordinates[1] - miny,
                                   Radius,
                                   label,
                                   color);
            }
            this->EndRenderingVertices();


            this->EndRenderingGraph();
        }


        void GraphRenderingContext::BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
        }
        void GraphRenderingContext::EndRenderingGraph()
        {
        }


        void GraphRenderingContext::BeginDeclaringVertices()
        {
            LastBrushColor = Color(0,0,0);
            this->SetBrushColor(LastBrushColor);
        }
        void GraphRenderingContext::DeclareVertex(int node_id, float x, float y, float radius, string text, Color color)
        {
        }
        void GraphRenderingContext::EndDeclaringVertices()
        {
        }


        void GraphRenderingContext::BeginRenderingEdges()
        {
            LastPenColor = Color(0,0,0);
            this->SetPenColor(LastPenColor);
            LastLineWidth = 1;
            this->SetLineWidth(LastLineWidth);
        }
        void GraphRenderingContext::RenderEdge(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, string text, Color color)
        {
            if(color != LastPenColor)
                this->SetPenColor(color);
            LastPenColor = color;
            if(width != LastLineWidth)
                this->SetLineWidth(width);
            LastLineWidth = width;
            this->Line(x1,y1,x2,y2);
            this->PutText((x1+x2)/2, (y1+y2)/2, text);
        }
        void GraphRenderingContext::RenderArc(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, string text, Color color)
        {
            if(color != LastPenColor)
                this->SetPenColor(color);
            LastPenColor = color;
            if(width != LastLineWidth)
                this->SetLineWidth(width);
            LastLineWidth = width;
            this->Arrow(x1, y1, x2, y2);
            this->PutText((x1+x2)/2, (y1+y2)/2, text);
        }
        void GraphRenderingContext::EndRenderingEdges()
        {
        }


        void GraphRenderingContext::BeginRenderingVertices()
        {
            LastBrushColor = Color(0,0,0);
            this->SetBrushColor(LastBrushColor);
        }
        void GraphRenderingContext::RenderVertex(int vertex_id, float x, float y, float radius, string text, Color color)
        {
            if(color != LastBrushColor)
                this->SetBrushColor(color);
            LastBrushColor = color;
            this->Circle(x,y,radius);
            this->PutText(x + radius*cos(M_PI/4), y+radius*sin(M_PI/4), text);
        }
        void GraphRenderingContext::EndRenderingVertices()
        {
        }

        // ==============================================================================================================

        GraphRenderingContextStream::GraphRenderingContextStream(std::ostream& os)
        {
            this->os = &os;
        }

    }
}
