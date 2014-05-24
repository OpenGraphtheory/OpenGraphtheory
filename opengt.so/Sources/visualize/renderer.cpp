
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
            VertexPartitioning vcoloringInt = G.GetVertexPartitioning(vertexcoloring);
            VertexColoring vertexColoring;
            for(VertexPartitioning::iterator i = vcoloringInt.begin(); i != vcoloringInt.end(); i++)
                vertexColoring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];

            EdgePartitioning ecoloringInt = G.GetEdgePartitioning(edgecoloring);
            EdgeColoring edgeColoring;
            for(EdgePartitioning::iterator i = ecoloringInt.begin(); i != ecoloringInt.end(); i++)
                edgeColoring[i->first] = Color::DefaultColors[i->second < Color::NumDefaultColors ? i->second : 0];

            RenderGraph(G, vertexColoring, edgeColoring, dpi, edgewidth, vertexradius);
        }

        void GraphRenderingContext::RenderGraph(Graph& G, VertexColoring& vertexcoloring, EdgeColoring& edgecoloring, float dpi,
                         float edgewidth, float vertexradius)
        {
            VertexIterator v1 = G.BeginVertices();
            Coordinates coordinates = (*v1)->GetCoordinates();
            float minx = coordinates[0], maxx = coordinates[1], maxy = coordinates[1], miny = coordinates[1];
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                coordinates = (*v1)->GetCoordinates();
                float radius = vertexradius >= 0 ? vertexradius : (*v1)->GetWeight();

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
            LastBrushColor = Color(0,0,0);
            this->SetBrushColor(LastBrushColor);
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = (*v)->GetCoordinates();
                Color color = vertexcoloring.find(*v) != vertexcoloring.end() ? vertexcoloring[*v] : Color(0,0,0);
                if(color != LastBrushColor)
                    this->SetBrushColor(color);
                LastBrushColor = color;

                float radius = vertexradius >= 0 ? vertexradius : (*v)->GetWeight();
                string label = Translator != NULL ? Translator->Translate((*v)->GetLabel()) : (*v)->GetLabel();
                this->DeclareVertex((*v)->GetID(),
                                    coordinates[0] - minx,
                                    coordinates[1] - miny,
                                    radius,
                                    label,
                                    color);
            }
            this->EndDeclaringVertices();


            // Render Edges
            this->BeginRenderingEdges();
            LastPenColor = Color(0,0,0);
            this->SetPenColor(LastPenColor);
            LastLineWidth = 1;
            this->SetLineWidth(LastLineWidth);
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                Coordinates FromCoordinates = (*e)->From()->GetCoordinates();
                Coordinates ToCoordinates = (*e)->To()->GetCoordinates();
                float x1 = FromCoordinates[0] - minx;
                float y1 = FromCoordinates[1] - miny;
                float x2 = ToCoordinates[0] - minx;
                float y2 = ToCoordinates[1] - miny;

                Color color = edgecoloring.find(*e) != edgecoloring.end() ? edgecoloring[*e] : Color(0,0,0);
                if(color != LastPenColor)
                    this->SetPenColor(color);
                LastPenColor = color;

                float LineWidth = edgewidth >= 0.0f ? edgewidth : (*e)->GetWeight();
                if(LineWidth != LastLineWidth)
                    this->SetLineWidth(LineWidth);
                LastLineWidth = LineWidth;

                string label = Translator != NULL ? Translator->Translate((*e)->GetLabel()) : (*e)->GetLabel();

                if((*e)->IsEdge())
                {
                    this->RenderEdge((*e)->From()->GetID(),
                                     (*e)->To()->GetID(),
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
                    float TargetRadius = vertexradius >= 0 ? vertexradius : (*e)->To()->GetWeight();
                    Visualization::Vector2D intersectionpoint(x2-x1, y2-y1);
                    intersectionpoint = intersectionpoint * ((intersectionpoint.Length()-TargetRadius) / intersectionpoint.Length());
                    intersectionpoint.x += x1;
                    intersectionpoint.y += y1;
                    this->RenderArc((*e)->From()->GetID(),
                                    (*e)->To()->GetID(),
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
            LastBrushColor = Color(0,0,0);
            this->SetBrushColor(LastBrushColor);
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = (*v)->GetCoordinates();
                float Radius = vertexradius >= 0 ? vertexradius : (*v)->GetWeight();

                Color color = vertexcoloring.find(*v) != vertexcoloring.end() ? vertexcoloring[*v] : Color(0,0,0);
                if(color != LastBrushColor)
                    this->SetBrushColor(color);
                LastBrushColor = color;

                string label = Translator != NULL ? Translator->Translate((*v)->GetLabel()) : (*v)->GetLabel();

                this->RenderVertex((*v)->GetID(),
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
        }
        void GraphRenderingContext::DeclareVertex(int node_id, float x, float y, float radius, string text, Color color)
        {
        }
        void GraphRenderingContext::EndDeclaringVertices()
        {
        }


        void GraphRenderingContext::BeginRenderingEdges()
        {
        }
        void GraphRenderingContext::RenderEdge(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, string text, Color color)
        {
            this->Line(x1,y1,x2,y2);
            this->PutText((x1+x2)/2, (y1+y2)/2, text);
        }
        void GraphRenderingContext::RenderArc(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, string text, Color color)
        {
            this->Arrow(x1, y1, x2, y2);
            this->PutText((x1+x2)/2, (y1+y2)/2, text);
        }
        void GraphRenderingContext::EndRenderingEdges()
        {
        }


        void GraphRenderingContext::BeginRenderingVertices()
        {
        }
        void GraphRenderingContext::RenderVertex(int vertex_id, float x, float y, float radius, string text, Color color)
        {
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
