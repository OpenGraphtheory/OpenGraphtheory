
#include <iostream>
#include <fstream>
#include <string>
#include "../opengt.h"
#include "import.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Import
    {

        ImportFilter::ImportFilter()
        {

        }

        ImportFilter::ImportFilter(string name, string description, string URL, Graph ImportFunction(istream& is))
        {
            this->name = name;
            this->description = description;
            this->URL = URL;
            this->ImportFunction = ImportFunction;
        }


        Graph ImportFilter::Import(istream& is)
        {
            return this->ImportFunction(is);
        }

        Graph ImportFilter::Import(string FileName)
        {
            ifstream is(FileName.c_str());
            return Import(is);
            // is.close(); done in the destructor anyways. no need to copy the result of Import(is)
        }



        Graph ImportFilter::Import(istream& is, string format)
        {
            map<string, ImportFilter> filters = GetImportFilters();

            if(filters.find(format) == filters.end())
                throw "unknown import-format";

            return filters[format].Import(is);
        }

        Graph ImportFilter::Import(string FileName, string format)
        {
            map<string, ImportFilter> filters = GetImportFilters();

            if(filters.find(format) == filters.end())
                throw "unknown import-format";

            return filters[format].Import(FileName);
        }



        map<string, ImportFilter> ImportFilter::GetImportFilters()
        {
            map<string, ImportFilter> result;

            /*
            result["mpost"] = ImportFilter("mpost",
                                     "MetaPost",
                                     "http://de.wikipedia.org/wiki/MetaPost",
                                     ImportFromMPOST);
            result["svg"] = ImportFilter("svg",
                                     "Scalable Vector Graphics",
                                     "http://de.wikipedia.org/wiki/Scalable_Vector_Graphics",
                                     ImportFromSVG);
            result["gastex"] = ImportFilter("gastex",
                                     "LaTeX package \"GasTeX\"",
                                     "http://www.lsv.ens-cachan.fr/~gastin/gastex/",
                                     ImportFromGASTEX);
            result["latex"] = ImportFilter("latex",
                                     "native LaTeX drawing routines",
                                     "",
                                     ImportFromLATEX);
            result["vrmlgraph"] = ImportFilter("vrmlgraph",
                                     "Virtual Reality Modeling Language",
                                     "http://vrmlgraph.i-scream.org.uk",
                                     ImportFromVRMLGRAPH);
            */

            result["tgf"] = ImportFilter("tgf",
                                     "Trivial Graph Format",
                                     "http://en.wikipedia.org/wiki/Trivial_Graph_Format",
                                     ImportFromTGF);

            /*
            result["tei"] = ImportFilter("tei",
                                     "Text Encoding Initiative",
                                     "http://www.tei-c.org/release/doc/tei-p5-doc/de/html/GD.html",
                                     ImportFromTEI);
            result["gml"] = ImportFilter("gml",
                                     "Graph Modeling Language",
                                     "http://www.fim.uni-passau.de/en/fim/faculty/chairs/theoretische-informatik/projects.html",
                                     ImportFromGML);
            result["rgml"] = ImportFilter("rgml",
                                     "Resource Description Framework (RDF) Graph Modeling Language",
                                     "http://www.cs.rpi.edu/research/groups/pb/punin/public_html/RGML/",
                                     ImportFromRGML);
            result["xgmml"] = ImportFilter("xgmml",
                                     "eXtensible Graph Markup and Modeling Language",
                                     "http://www.cs.rpi.edu/research/groups/pb/punin/public_html/XGMML/",
                                     ImportFromXGMML);
            result["dot"] = ImportFilter("dot",
                                     "Fileformat of the Graphviz project",
                                     "http://www.graphviz.org/doc/info/lang.html",
                                     ImportFromDOT);
            result["graphml"] = ImportFilter("graphml",
                                     "GraphML",
                                     "http://graphml.graphdrawing.org/",
                                     ImportFromGRAPHML);
            */
            result["gxl"] = ImportFilter("gxl",
                                     "Graph eXchange Language",
                                     "http://www.gupro.de/GXL/",
                                     ImportFromGXL);

            return result;
        }

        Graph ImportFromGXL(istream& is)
        {
            Graph G;
            is >> G;
            return G;
        }

    }
}

