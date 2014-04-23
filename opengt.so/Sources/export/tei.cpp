
#include "../../Headers/export/tei.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterTEI::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring,
                                     map<Graph::EdgeIterator, Color>& edgecoloring, float dpi, float edgewidth,
                                     float vertexradius)
        {
            if(G.IsHypergraph())
                throw "The TEI fileformat doesn\'t support hypergraphs\n";
            if(G.IsMixed())
                throw "The TEI fileformat doesn\'t support mixed graphs\n";
            bool directed = G.IsDirected();

            StringTranslatorXML Translator;

            os << "<!-- www.Open-Graphtheory.org -->\n";
            os << "<graph type=\"" << (directed?"":"un") << "directed\">\n";
            os << "  <label>" << Translator.Translate(G.GetLabel()) << "</label>\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "  <node xml:id=\"" << v.GetID() << "\">\n";
                os << "    <label>" << Translator.Translate(v.GetLabel()) << "</label>\n";
                os << "  </node>\n";
            }

            /// write edges or arcs (distinguishable only by the "type" attribute of the graph!)
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "  <arc from=\"#" << e.From().GetID() << "\" to=\"#" << e.To().GetID() << "\">\n";
                os << "    <label>" << Translator.Translate(e.GetLabel()) << "</label>\n";
                os << "  </arc>\n";
            }

            os << "</graph>\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterTEI::ExportFilterTeiRegistrator(&ExportFilter::ExportFilterFactory, "tei",
            new DefaultInstantiator<ExportFilter, ExportFilterTEI>("tei", "Text Encoding Initiative",
                "http://www.tei-c.org/release/doc/tei-p5-doc/de/html/GD.html"));


    }
}
