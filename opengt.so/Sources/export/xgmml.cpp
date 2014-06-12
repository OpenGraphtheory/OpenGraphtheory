
#include "../../Headers/export/xgmml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterXGMML::Export(Graph& G, ostream& os, VertexColoring& vertexcoloring,
                                       EdgeColoring& edgecoloring,  float dpi, float edgewidth, float vertexradius)
        {
            if(G.IsHypergraph())
                throw "The XGMML fileformat does not support hypergraphs\n";

            StringTranslatorXML Translator;

            os << "<?xml version=\"1.0\"?>\n";
            os << "<!DOCTYPE graph PUBLIC \"-//John Punin//DTD graph description//EN\" \"http://www.cs.rpi.edu/~puninj/XGMML/xgmml.dtd\">\n";
            os << "<!-- www.Open-Graphtheory.org -->\n";

            os << "<graph label=\"" << Translator.Translate(G.GetLabel()) << "\">\n";

            /// write vertices
            for(VertexIterator vi = G.BeginVertices(); vi != G.EndVertices(); vi++)
            {
                Vertex* v = *vi;
                Coordinates coordinates = v->GetCoordinates(0);
                os << "  <node id=\"" << v->GetID() << "\" label=\"" << Translator.Translate(v->GetLabel())
                   << "\" weight=\"" << v->GetWeight() << "\">\n";
                if(coordinates.size() >= 2)
                    os << "    <graphics x=\"" << coordinates[0] << "\" y=\"" << coordinates[1] << "\"/>\n";
                os << "  </node>\n";
            }

            /// write edges
            for(EdgeIterator ei = G.BeginEdges(); ei != G.EndEdges(); ei++)
            {
                Edge* e = *ei;
                os << "  <edge source=\"" << e->From()->GetID() << "\" target=\""<< e->To()->GetID()
                   << "\" weight=\"" << e->GetWeight() << "\" label=\"" << Translator.Translate(e->GetLabel()) << "\">\n";
                os << "    <graphics type=\"" << (e->IsEdge()?"line":"arc") << "\"/>\n";
                os << "  </edge>\n";
            }

            os << "</graph>\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterXGMML::ExportFilterXgmmlRegistrator(&ExportFilter::ExportFilterFactory, "xgmml",
            new DefaultInstantiator<ExportFilter, ExportFilterXGMML>("xgmml", "eXtensible Graph Markup and Modeling Language",
                "http://cgi5.cs.rpi.edu/research/groups/pb/punin/public_html/XGMML/"));


    }
}
