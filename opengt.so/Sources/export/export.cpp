
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


        void ExportFilter::DoExport(Graph& G, ostream& os, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
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
                    else
                    {
                        BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
                        if(battr != NULL && battr->Value)
                            vcoloring[v] = Color::DefaultColors[0];
                    }
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
                    else
                    {
                        BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
                        if(battr != NULL && battr->Value)
                            ecoloring[e] = Color::DefaultColors[0];
                    }
                }
            }

            Export(G, os, vcoloring, ecoloring, dpi, edgewidth, vertexradius);
        }


        void ExportFilter::DoExport(Graph& G, string FileName, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            ofstream os(FileName.c_str());
            DoExport(G, os, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
            os.close();
        }

        string ExportFilter::DoExport(Graph& G, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            stringstream sstream;
            DoExport(G, sstream, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
            return sstream.str();
        }



        void ExportFilter::Export(Graph& G, ostream& os, string format, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->DoExport(G, os, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
            delete exportfilter;
        }

        void ExportFilter::Export(Graph& G, string FileName, string format, std::string vertexcoloring, std::string edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->DoExport(G, FileName, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
            delete exportfilter;
        }


        void GraphicalExportFilter::Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                           map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                           map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi,
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


            // Write Header
            Begin(os, maxx-minx, maxy-miny, dpi);
            Visualization::Color LastColor(0,0,0);


            BeginDeclaringVertices(os);

            SetPenColor(os, LastColor);
            SetBrushColor(os, LastColor);

            // Write Vertex-Declaration (if necessary)
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                coordinates = v.GetCoordinates();

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

                DeclareVertex(os, v.GetID(), coordinates[0] - minx, coordinates[1] - miny, vertexradius >= 0 ? vertexradius : v.GetWeight(), v.GetLabel());
            }

            EndDeclaringVertices(os);


            // Render Edges
            BeginRenderingEdges(os);

            LastColor = Visualization::Color(0,0,0);
            SetPenColor(os, Visualization::Color(0,0,0));

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

                float Width = edgewidth >= 0.0f ? edgewidth : e.GetWeight();
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
                        SetPenColor(os, LastColor);
                    }
                }
                else
                {
                    if(LastColor != Visualization::Color(0,0,0))
                    {
                        LastColor = Visualization::Color(0,0,0);
                        SetPenColor(os, LastColor);
                    }
                }

                if(e.IsEdge())
                    Line(os, e.From().GetID(), e.To().GetID(), x1, y1, x2, y2);
                else
                    Arrow(os, e.From().GetID(), e.To().GetID(), x1, y1, x2, y2);


                if(e.GetLabel() != "")
                    PutEdgeText(os, e.From().GetID(), e.To().GetID(), (x1+x2)/2, (y1+y2)/2, e.GetLabel());
            }

            EndRenderingEdges(os);


            // Render Vertices
            BeginRenderingVertices(os);

            LastColor = Visualization::Color(0,0,0);
            SetBrushColor(os, Visualization::Color(0,0,0));
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> Coordinates = v.GetCoordinates();
                float x = Coordinates[0] - minx, y = Coordinates[1] - miny;
                float Radius = vertexradius >= 0 ? vertexradius : v.GetWeight();

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

            EndRenderingVertices(os);


            End(os);

        }


        void GraphicalExportFilter::Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI)
        {

        }

        void GraphicalExportFilter::End(ostream &os)
        {

        }

        void GraphicalExportFilter::BeginDeclaringVertices(ostream &os)
        {

        }

        void GraphicalExportFilter::DeclareVertex(ostream &os, int node_id, float x, float y, float radius, string text)
        {

        }

        void GraphicalExportFilter::EndDeclaringVertices(ostream &os)
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



        void GraphicalExportFilter::BeginRenderingEdges(ostream &os)
        {

        }

        void GraphicalExportFilter::Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {

        }

        void GraphicalExportFilter::Arrow(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {
            Line(os, from_id, to_id, x1, y1, x2, y2);
        }

        void GraphicalExportFilter::EndRenderingEdges(ostream &os)
        {

        }



        void GraphicalExportFilter::BeginRenderingVertices(ostream &os)
        {

        }

        void GraphicalExportFilter::Circle(ostream &os, int node_id, float x, float y, float radius)
        {

        }

        void GraphicalExportFilter::EndRenderingVertices(ostream &os)
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




        string ExportFilter::SanitizeString(string String)
        {
            map<char, string> SpecialCharacterMap = SpecialCharacters();
            string result;
            for(string::iterator i = String.begin(); i != String.end(); i++)
            {
                if((int)*i == -61)
                {
                    i++;
                    switch((int)*i)
                    {
                        // ü
                        case -68: result += SpecialCharacter_uuml(); break;
                        // ö
                        case -74: result += SpecialCharacter_ouml(); break;
                        // ä
                        case -92: result += SpecialCharacter_auml(); break;
                        // Ä
                        case -124: result += SpecialCharacter_Auml(); break;
                        // Ö
                        case -106: result += SpecialCharacter_Ouml(); break;
                        // Ü
                        case -100: result += SpecialCharacter_Uuml(); break;
                        // ß
                        case -97: result += SpecialCharacter_szlig(); break;
                    }
                    continue;
                }

                if(SpecialCharacterMap.find(*i) != SpecialCharacterMap.end())
                    result += SpecialCharacterMap[*i];
                else
                    result += *i;
            }
            return result;
        }

        map<char, string> ExportFilter::SpecialCharacters()
        {
            map<char, string> result;
            return result;
        }
        string ExportFilter::SpecialCharacter_auml()
        {
            return "ä";
        }
        string ExportFilter::SpecialCharacter_Auml()
        {
            return "Ä";
        }
        string ExportFilter::SpecialCharacter_uuml()
        {
            return "ü";
        }
        string ExportFilter::SpecialCharacter_Uuml()
        {
            return "Ü";
        }
        string ExportFilter::SpecialCharacter_ouml()
        {
            return "ö";
        }
        string ExportFilter::SpecialCharacter_Ouml()
        {
            return "Ö";
        }
        string ExportFilter::SpecialCharacter_szlig()
        {
            return "ß";
        }


    }
}
