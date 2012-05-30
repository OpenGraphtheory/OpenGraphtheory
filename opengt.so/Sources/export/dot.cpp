
#include "../../Headers/export/dot.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterDOT::Export(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The DOT fileformat doesn\'t support hypergraphs\n";

            os << "// www.open-graphtheory.org\n";
            os << "graph ogt2dot_" << G.GetID() << " {\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "\tv" << v.GetID() << " [ label=\"" << v.GetLabel() << "\"";
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() > 1)
                {
                    os << ", pos=\"" << coordinates[0];
                    for(unsigned int i = 1; i < coordinates.size(); i++)
                        os << "," << coordinates[i];
                    os << "\"";
                }

                os << " ];\n";
            }

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                if(e.IsEdge())
                {
                    os << "\tv" << e.From().GetID() << " -- v" << e.To().GetID()
                       << " [ label=\"" << e.GetLabel() << "\" dir=\"none\" ];\n";
                }
                else
                {
                    os << "\tv" << e.From().GetID() << " -> v" << e.To().GetID()
                       << " [ label=\"" << e.GetLabel() << "\" dir=\"forward\" ];\n";
                }
            }

            os << "}\n";
        }

//        FactoryRegistrator<ExportFilter> ExportFilterDotRegistrator(&ExportFilter::ExportFilterFactory, "dot",
//            new DefaultInstantiator<ExportFilter, ExportFilterDOT>("dot", "Fileformat of the Graphviz project", "http://www.graphviz.org/doc/info/lang.html"));

    }
}
