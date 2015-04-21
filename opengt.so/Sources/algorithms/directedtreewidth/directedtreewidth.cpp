
#include "../../../Headers/algorithms/directedtreewidth/directedtreewidth.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace DirectedTreewidth
        {
            MultiFactoryRegistrator<Algorithm> AlgorithmDIRECTEDTREEDECOMPOSITION::AlgorithmDirectedTreeDecompositionRegistrator(
                &Algorithm::AlgorithmFactory, "directedtreedecomposition", new DefaultInstantiator<Algorithm, AlgorithmDIRECTEDTREEDECOMPOSITION>(
                "directedtreedecomposition", "returns a directed tree-decomposition of the graph", "http://dx.doi.org/10.1006/jctb.2000.2031"));


            bool AlgorithmDIRECTEDTREEDECOMPOSITION::FindWeaklyBalancedWSeparation(VertexSet& X, VertexSet& S, VertexSet& Y)
            {

                return false;
            }

            void AlgorithmDIRECTEDTREEDECOMPOSITION::Run(Graph& G, std::vector<std::string> parameters)
            {
                if(parameters.size() < 3)
                    throw "directed treewidth algorithm needs 3 parameters (source, drain, result name)";

                string SourceName = parameters[0];
                string DrainName = parameters[1];
                string MaximumFlowName = parameters[2];

                VertexIterator Source = G.EndVertices();
                VertexIterator Drain = G.EndVertices();
                for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                {
                    if(SourceName == (*v)->GetLabel())
                        Source = v;
                    if(DrainName == (*v)->GetLabel())
                        Drain = v;
                }

                if(Source == G.EndVertices())
                    throw "directed treewidth algorithm: source not found";
                if(Drain == G.EndVertices())
                    throw "directed treewidth algorithm: drain not found";

                //AddMaximumVertexFlow(G, *Source, *Drain, MaximumFlowName);
            }

        }
    }
}
