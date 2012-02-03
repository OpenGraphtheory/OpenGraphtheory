
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include "../opengt.h"
#include "export.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        ExportFilter::ExportFilter()
        {

        }

        ExportFilter::ExportFilter(string name, string description, string URL, void ExportFunction(Graph& G, ostream& os))
        {
            this->name = name;
            this->description = description;
            this->URL = URL;
            this->ExportFunction = ExportFunction;
        }


        void ExportFilter::Export(Graph& G, ostream& os)
        {
            this->ExportFunction(G,os);
        }

        void ExportFilter::Export(Graph& G, string FileName)
        {
            ofstream os(FileName.c_str());
            Export(G, os);
            os.close();
        }

        string ExportFilter::Export(Graph& G)
        {
            stringstream sstream;
            Export(G, sstream);
            return sstream.str();
        }



        void ExportFilter::Export(Graph& G, ostream& os, string format)
        {
            map<string, ExportFilter> filters = GetExportFilters();

            if(filters.find(format) == filters.end())
                throw "unknown export-format";

            filters[format].Export(G, os);
        }

        void ExportFilter::Export(Graph& G, string FileName, string format)
        {
            map<string, ExportFilter> filters = GetExportFilters();

            if(filters.find(format) == filters.end())
                throw "unknown export-format";

            filters[format].Export(G, FileName);
        }



        map<string, ExportFilter> ExportFilter::GetExportFilters()
        {
            map<string, ExportFilter> result;

            result["mpost"] = ExportFilter("mpost",
                                     "MetaPost",
                                     "http://de.wikipedia.org/wiki/MetaPost",
                                     ExportToMPOST);
            result["svg"] = ExportFilter("svg",
                                     "Scalable Vector Graphics",
                                     "http://de.wikipedia.org/wiki/Scalable_Vector_Graphics",
                                     ExportToSVG);
            result["gastex"] = ExportFilter("gastex",
                                     "LaTeX package \"GasTeX\"",
                                     "http://www.lsv.ens-cachan.fr/~gastin/gastex/",
                                     ExportToGASTEX);
/*          TODO
            result["latex"] = ExportFilter("latex",
                                     "native LaTeX drawing routines",
                                     "",
                                     ExportToLATEX);
*/
            result["vrmlgraph"] = ExportFilter("vrmlgraph",
                                     "Virtual Reality Modeling Language",
                                     "http://vrmlgraph.i-scream.org.uk",
                                     ExportToVRMLGRAPH);
            result["tgf"] = ExportFilter("tgf",
                                     "Trivial Graph Format",
                                     "http://en.wikipedia.org/wiki/Trivial_Graph_Format",
                                     ExportToTGF);
            result["tei"] = ExportFilter("tei",
                                     "Text Encoding Initiative",
                                     "http://www.tei-c.org/release/doc/tei-p5-doc/de/html/GD.html",
                                     ExportToTEI);
            result["gml"] = ExportFilter("gml",
                                     "Graph Modeling Language",
                                     "http://www.fim.uni-passau.de/en/fim/faculty/chairs/theoretische-informatik/projects.html",
                                     ExportToGML);
            result["rgml"] = ExportFilter("rgml",
                                     "Resource Description Framework (RDF) Graph Modeling Language",
                                     "http://www.cs.rpi.edu/research/groups/pb/punin/public_html/RGML/",
                                     ExportToRGML);
            result["xgmml"] = ExportFilter("xgmml",
                                     "eXtensible Graph Markup and Modeling Language",
                                     "http://www.cs.rpi.edu/research/groups/pb/punin/public_html/XGMML/",
                                     ExportToXGMML);
            result["dot"] = ExportFilter("dot",
                                     "Fileformat of the Graphviz project",
                                     "http://www.graphviz.org/doc/info/lang.html",
                                     ExportToDOT);
            result["graphml"] = ExportFilter("graphml",
                                     "GraphML",
                                     "http://graphml.graphdrawing.org/",
                                     ExportToGRAPHML);
            result["gxl"] = ExportFilter("gxl",
                                     "Graph eXchange Language",
                                     "http://www.gupro.de/GXL/",
                                     ExportToGXL);

            return result;
        }

        void ExportToGXL(Graph& G, ostream& os)
        {
            os << G;
        }

    }
}
