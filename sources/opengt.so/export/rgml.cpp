
#include <iostream>
#include "../opengt.h"
#include "export.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterRGML::Export(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The XGMML fileformat does not support hypergraphs\n";

            os << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
            os << "<!-- www.Open-Graphtheory.org -->\n\n";

            os << "<rdf:RDF\n"
               << "  xmlns:rdf=\"http://www.w3.org/1999/02/22-rdf-syntax-ns#\"\n"
               << "  xmlns=\"http://purl.org/puninj/2001/05/rgml-schema#\"\n"
               << "  xmlns:rgml=\"http://purl.org/puninj/2001/05/rgml-schema#\"\n"
               << ">\n\n";

            os << "  <Graph rdf:ID=\"g" << G.GetID() << "\" rgml:directed=\"false\">\n";

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
                os << "  <Node rdf:ID=\"n" << v.GetID() << "\"/>\n";

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "\n  <Edge rdf:ID=\"e" << e.GetID() << "\" directed=\"" << (e.IsEdge()?"false":"true") << "\">\n";
                os << "    <source rdf:resource=\"#n" << e.From().GetID() << "\"/>\n";
                os << "    <target rdf:resource=\"#n" << e.To().GetID() << "\"/>\n";
                os << "  </Edge>\n";
            }

            os << "\n</rdf:RDF>\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterRgmlRegistrator(&ExportFilter::ExportFilterFactory, "rgml",
            new DefaultInstantiator<ExportFilter, ExportFilterRGML>("rgml", "Resource Description Framework (RDF) Graph Modeling Language",
                "http://www.cs.rpi.edu/research/groups/pb/punin/public_html/RGML/"));


    }
}
