
#include "../../Headers/export/prolog.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterPROLOG::Export(Graph& G, ostream& os, VertexColoring& vertexcoloring,
                                        EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            os << "\n%  www.Open-Graphtheory.org\n\n";

            StringTranslator Translator; // should be replaced with an appropriate translator

            /// write vertices
            os << "?- dynamic(vertex/3).\n";
            for(VertexIterator vi = G.BeginVertices(); vi != G.EndVertices(); vi++)
            {
                Vertex* v= *vi;
                os << "vertex(" << v->GetID() << ", '"<< Translator.Translate(v->GetLabel()) << "', " << v->GetWeight() << ").\n";
            }
            os << "v(X) :- vertex(X,_,_).\n\n";

             /// write edges and arcs
            os << "?- dynamic(edge/4).\n";
            os << "?- dynamic(arc/4).\n";
            for(EdgeIterator ei = G.BeginEdges(); ei != G.EndEdges(); ei++)
            {
                Edge* e = *ei;
                os << (e->IsDirected()?"arc":"edge") << "(" << e->From()->GetID() << ", " << e->To()->GetID()
                   << ", '" << Translator.Translate(e->GetLabel()) << "', " << e->GetWeight() << ").\n";
            }
            os << "a(X,Y) :- arc(X,Y,_,_).\n";
            os << "e(X,Y) :- edge(X,Y,_,_).\n";
            os << "e(X,Y) :- edge(Y,X,_,_).\n";
            os << "r(X,Y) :- e(X,Y).\n";
            os << "r(X,Y) :- a(X,Y).\n\n";

        }

        FactoryRegistrator<ExportFilter> ExportFilterPROLOG::ExportFilterPrologRegistrator(&ExportFilter::ExportFilterFactory, "prolog",
            new DefaultInstantiator<ExportFilter, ExportFilterPROLOG>("prolog", "Prolog Source Code",
                "http://en.wikipedia.org/wiki/Prolog"));
    }
}
