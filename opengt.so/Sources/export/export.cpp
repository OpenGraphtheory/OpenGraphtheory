
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
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


        void ExportFilter::DoExport(Graph& G, ostream& os, std::string vertexcoloring, std::string edgecoloring)
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

            Export(G, os, vcoloring, ecoloring);
        }


        void ExportFilter::DoExport(Graph& G, string FileName, std::string vertexcoloring, std::string edgecoloring)
        {
            ofstream os(FileName.c_str());
            DoExport(G, os, vertexcoloring, edgecoloring);
            os.close();
        }

        string ExportFilter::DoExport(Graph& G, std::string vertexcoloring, std::string edgecoloring)
        {
            stringstream sstream;
            DoExport(G, sstream, vertexcoloring, edgecoloring);
            return sstream.str();
        }



        void ExportFilter::Export(Graph& G, ostream& os, string format, std::string vertexcoloring, std::string edgecoloring)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->DoExport(G, os, vertexcoloring, edgecoloring);
            delete exportfilter;
        }

        void ExportFilter::Export(Graph& G, string FileName, string format, std::string vertexcoloring, std::string edgecoloring)
        {
            ExportFilter* exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
            if(exportfilter == NULL)
                throw "unknown export-format";

            exportfilter->DoExport(G, FileName, vertexcoloring, edgecoloring);
            delete exportfilter;
        }



    }
}
