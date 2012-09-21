
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <math.h>
#include "../../Headers/export/export.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        Factory<ExportFilter> ExportFilter::ExportFilterFactory;

        ExportFilter::~ExportFilter()
        {

        }


        void ExportFilter::DoExport(Graph& G, ostream& os, std::string vertexcoloring, std::string edgecoloring, float dpi)
        {
            map<Graph::VertexIterator, Color> vcoloring;
            map<Graph::EdgeIterator, Color> ecoloring;

            if(vertexcoloring != "")
            {
                for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                {
                    Attribute* attr = v.Attributes().GetAttribute(vertexcoloring);
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        vcoloring[v] = Color::DefaultColors[iattr->Value < 7 ? iattr->Value : 0];
                }
            }

            if(edgecoloring != "")
            {
                for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                {
                    Attribute* attr = e.Attributes().GetAttribute(edgecoloring);
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        ecoloring[e] = Color::DefaultColors[iattr->Value < 7 ? iattr->Value : 0];
                }
            }

            Export(G, os, vcoloring, ecoloring, dpi);
        }


        void ExportFilter::DoExport(Graph& G, string FileName, std::string vertexcoloring, std::string edgecoloring, float dpi)
        {
            ofstream os(FileName.c_str());
            DoExport(G, os, vertexcoloring, edgecoloring, dpi);
            os.close();
        }

        string ExportFilter::DoExport(Graph& G, std::string vertexcoloring, std::string edgecoloring, float dpi)
        {
            stringstream sstream;
            DoExport(G, sstream, vertexcoloring, edgecoloring, dpi);
            return sstream.str();
        }



        void ExportFilter::Export(Graph& G, ostream& os, string format, std::string vertexcoloring, std::string edgecoloring, float dpi)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->DoExport(G, os, vertexcoloring, edgecoloring, dpi);
            delete exportfilter;
        }

        void ExportFilter::Export(Graph& G, string FileName, string format, std::string vertexcoloring, std::string edgecoloring, float dpi)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->DoExport(G, FileName, vertexcoloring, edgecoloring, dpi);
            delete exportfilter;
        }


        void GraphicalExportFilter::Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                           map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                           map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi)
        {
            Graph::VertexIterator v1 = G.BeginVertices();
            vector<float> coordinates = v1.GetCoordinates();
            float minx = coordinates[0], maxx = coordinates[1], maxy = coordinates[1], miny = coordinates[1];
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                coordinates = v1.GetCoordinates();
                float radius = v1.GetWeight();

                if(coordinates[0]-radius < minx)
                    minx = coordinates[0]-radius;
                if(coordinates[1]-radius < miny)
                    miny = coordinates[1]-radius;
                if(coordinates[0]+radius > maxx)
                    maxx = coordinates[0]+radius;
                if(coordinates[1]+radius > maxy)
                    maxy = coordinates[1]+radius;
            }


            // Write Header
            Begin(os, maxx-minx, maxy-miny, dpi);
            Visualization::Color LastColor(0,0,0);
            SetPenColor(os, LastColor);


            // Write Vertex-Declaration (if necessary)
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();
                DeclareNode(os, v.GetID(), coordinates[0] - minx, coordinates[1] - miny);
            }


            // Render Edges
            float LastWidth = 1;
            SetLineWidth(os, 1);
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                vector<float> FromCoordinates = e.From().GetCoordinates();
                vector<float> ToCoordinates = e.To().GetCoordinates();
                float x1 = FromCoordinates[0] - minx; // + e.From().GetWeight();
                float y1 = FromCoordinates[1] - miny; // + e.From().GetWeight();
                float x2 = ToCoordinates[0] - minx; // + e.To().GetWeight();
                float y2 = ToCoordinates[1] - miny; // + e.To().GetWeight();

                float Width = e.GetWeight();
                if(Width != LastWidth)
                {
                    SetLineWidth(os, Width);
                    LastWidth = Width;
                }

                // color
                if(edgecoloring.find(e) != edgecoloring.end())
                {
                    if(LastColor != edgecoloring[e])
                    {
                        LastColor = edgecoloring[e];
                        SetPenColor(os, edgecoloring[e]);
                    }
                }
                else
                {
                    if(LastColor != Visualization::Color(0,0,0))
                    {
                        LastColor = Visualization::Color(0,0,0);
                        SetPenColor(os, Visualization::Color(0,0,0));
                    }
                }


                if(e.IsEdge())
                    Line(os, e.From().GetID(), e.To().GetID(), x1, y1, x2, y2);
                else
                    Arrow(os, e.From().GetID(), e.To().GetID(), x1, y1, x2, y2);


                if(e.GetLabel() != "")
                    PutEdgeText(os, e.From().GetID(), e.To().GetID(), (x1+x2)/2, (y1+y2)/2, e.GetLabel());
            }

            // Render Vertices
            LastColor = Visualization::Color(0,0,0);
            SetBrushColor(os, Visualization::Color(0,0,0));
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> Coordinates = v.GetCoordinates();
                float x = Coordinates[0] - minx, y = Coordinates[1] - miny;
                float Radius = v.GetWeight();

                // color
                if(vertexcoloring.find(v) != vertexcoloring.end())
                {
                    if(LastColor != vertexcoloring[v])
                    {
                        LastColor = vertexcoloring[v];
                        SetBrushColor(os, vertexcoloring[v]);
                    }
                }
                else
                {
                    if(LastColor != Visualization::Color(0,0,0))
                    {
                        LastColor = Visualization::Color(0,0,0);
                        SetBrushColor(os, Visualization::Color(0,0,0));
                    }
                }

                Circle(os, v.GetID(), x, y, Radius);

                if(v.GetLabel() != "")
                    PutVertexText(os, v.GetID(), x + sin(M_PI/4)*Radius, y + sin(M_PI/4)*Radius, v.GetLabel());
            }

            End(os);

        }


        void GraphicalExportFilter::Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI)
        {

        }

        void GraphicalExportFilter::End(ostream &os)
        {

        }

        void GraphicalExportFilter::DeclareNode(ostream &os, int node_id, float x, float y)
        {

        }

        void GraphicalExportFilter::SetPenColor(ostream &os, Visualization::Color color)
        {

        }

        void GraphicalExportFilter::SetBrushColor(ostream &os, Visualization::Color color)
        {

        }

        void GraphicalExportFilter::SetLineWidth(ostream &os, float width)
        {

        }

        void GraphicalExportFilter::Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {

        }

        void GraphicalExportFilter::Arrow(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {
            Line(os, from_id, to_id, x1, y1, x2, y2);
        }

        void GraphicalExportFilter::Circle(ostream &os, int node_id, float x, float y, float radius)
        {

        }

        void GraphicalExportFilter::PutText(ostream &os, float x, float y, string text)
        {

        }

        void GraphicalExportFilter::PutEdgeText(ostream &os, int from_id, int to_id, float x, float y, string text)
        {
            PutText(os, x, y, text);
        }

        void GraphicalExportFilter::PutVertexText(ostream &os, int node_id, float x, float y, string text)
        {
            PutText(os, x, y, text);
        }

    }
}
