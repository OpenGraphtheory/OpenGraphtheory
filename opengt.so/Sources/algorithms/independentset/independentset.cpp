
#include "../../../Headers/algorithms/independentset/independentset.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmINDEPENDENTSET::AlgorithmIndependentsetRegistrator(
            &Algorithm::AlgorithmFactory, "independentset", new DefaultInstantiator<Algorithm, AlgorithmINDEPENDENTSET>(
            "independentset", "Adds an independent set to the graph", "http://en.wikipedia.org/wiki/Independent_set_(graph_theory)"));
        MultiFactoryRegistrator<Algorithm> AlgorithmINDEPENDENTSET::AlgorithmStablesetRegistrator(
            &Algorithm::AlgorithmFactory, "stableset", new DefaultInstantiator<Algorithm, AlgorithmINDEPENDENTSET>(
            "stableset", "Adds a stable set (also known as independent set) to the graph", "http://en.wikipedia.org/wiki/Independent_set_(graph_theory)"));


        bool AlgorithmINDEPENDENTSET::FindIndependentSet(Graph& G, VertexSet& IndependentSet, unsigned int k)
        {
            VertexSet V;
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                V.insert(*v);

            return TestClique(IndependentSet, V, k, false);
        }


        void AlgorithmINDEPENDENTSET::AddIndependentSet(Graph &G, string IndependentSetName)
        {
            VertexSet LastIndependentSet;
            VertexSet CurrentIndependentSet;

            if(G.NumberOfVertices() <= 0)
                return;
            CurrentIndependentSet.insert(*G.BeginVertices());

            for(unsigned int k = 2; ; k++)
            {
                if(!FindIndependentSet(G, CurrentIndependentSet, k))
                    break;

                LastIndependentSet = CurrentIndependentSet;
                CurrentIndependentSet.clear();
            }

            G.AddVertexSet(LastIndependentSet, IndependentSetName);
        }

        void AlgorithmINDEPENDENTSET::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string IndependentSetName =  parameters[0];
            AddIndependentSet(G, IndependentSetName);
        }

    }
}

