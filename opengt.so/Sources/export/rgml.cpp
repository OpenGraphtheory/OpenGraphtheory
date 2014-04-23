
#include "../../Headers/export/rgml.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterRGML::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring,
                                      map<Graph::EdgeIterator, Color>& edgecoloring, float dpi, float edgewidth,
                                      float vertexradius)
        {

            StringTranslatorXML Translator;

            os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            os << "<!-- www.Open-Graphtheory.org -->\n\n";

            os << "<rdf:RDF\n"
               << "  xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
               << "  xmlns=\"http://purl.org/puninj/2001/05/rgml-schema#\"\n"
               << "  xmlns:rgml=\"http://purl.org/puninj/2001/05/rgml-schema#\"\n"
               << ">\n\n";

            os << "  <Graph rdf:ID=\"graph\" rgml:directed=\"false\">\n";

            /// declare vertices
            os << "    <nodes>\n";
            os << "      <rdf:Bag>\n";
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                os << "        <rdf:li rdf:resource=\"#n" << v.GetID() << "\"/>\n";
            os << "      </rdf:Bag>\n";
            os << "    </nodes>\n\n";

            /// declare edges and arcs
            os << "    <edges>\n";
            os << "      <rdf:Bag>\n";
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << "        <rdf:li rdf:resource=\"#e" << e.GetID() << "\"/>\n";
            os << "      </rdf:Bag>\n";
            os << "    </edges>\n";

            os << "  </Graph>\n\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                os << "  <Node rdf:ID=\"n" << v.GetID() << "\" rgml:label=\"" << Translator.Translate(v.GetLabel()) << "\"/>\n";

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                if(e.IsHyperedge())
                    continue;
                os << "\n  <Edge rdf:ID=\"e" << e.GetID() << "\" directed=\"" << (e.IsEdge()?"false":"true") << "\""
                   << " rgml:label=\"" << Translator.Translate(e.GetLabel()) << "\">\n";
                os << "    <source rdf:resource=\"#n" << e.From().GetID() << "\"/>\n";
                os << "    <target rdf:resource=\"#n" << e.To().GetID() << "\"/>\n";
                os << "  </Edge>\n";
            }

            /// write hyperedges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "\n  <Edge rdf:ID=\"e" << e.GetID() << "\""
                   << " rgml:label=\"" << Translator.Translate(e.GetLabel()) << "\">\n";
                os << "    <nodes>\n";
                os << "      <rdf:Seq>\n";
                for(Graph::VertexIterator inc = e.BeginIncidentVertices(); inc != e.EndIncidentVertices(); inc++)
                    os << "        <rdf:li rdf:resource=\"#n" << inc.GetID() << "\"/>\n";
                os << "      </rdf:Seq>\n";
                os << "    </nodes>\n";
                os << "  </Edge>\n";
            }

            os << "\n</rdf:RDF>\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterRGML::ExportFilterRgmlRegistrator(&ExportFilter::ExportFilterFactory, "rgml",
            new DefaultInstantiator<ExportFilter, ExportFilterRGML>("rgml", "Resource Description Framework (RDF) Graph Modeling Language",
                "http://www.cs.rpi.edu/research/groups/pb/punin/public_html/RGML/"));


    }
}
