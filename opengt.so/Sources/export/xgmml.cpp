
#include "../../Headers/export/xgmml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterXGMML::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi)
        {
            if(G.IsHypergraph())
                throw "The XGMML fileformat does not support hypergraphs\n";

            os << "<?xml version=\"1.0\"?>\n";
            os << "<!DOCTYPE graph PUBLIC \"-//John Punin//DTD graph description//EN\" \"http://www.cs.rpi.edu/~puninj/XGMML/xgmml.dtd\">\n";
            os << "<!-- www.Open-Graphtheory.org -->\n";

            os << "<graph id=\"" << G.GetID() << "\" label=\"" << G.GetLabel() << "\">\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                os << "  <node id=\"" << v.GetID() << "\" label=\"" << v.GetLabel()
                   << "\" weight=\"" << v.GetWeight() << "\">\n";
                os << "    <graphics x=\"" << coordinates[0] << "\" y=\"" << coordinates[1] << "\"/>\n";
                os << "  </node>\n";
            }

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "  <edge source=\"" << e.From().GetID() << "\" target=\""<< e.To().GetID()
                   << "\" weight=\"" << e.GetWeight() << "\" label=\"" << e.GetLabel() << "\">\n";
                os << "    <graphics type=\"" << (e.IsEdge()?"line":"arc") << "\"/>\n";
                os << "  </edge>\n";
            }

            os << "</graph>\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterXGMML::ExportFilterXgmmlRegistrator(&ExportFilter::ExportFilterFactory, "xgmml",
            new DefaultInstantiator<ExportFilter, ExportFilterXGMML>("xgmml", "eXtensible Graph Markup and Modeling Language",
                "http://www.cs.rpi.edu/research/groups/pb/punin/public_html/XGMML/"));


    }
}