
#include "../../../Headers/algorithms/vertexcover/vertexcoverAPX2.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        MultiFactoryRegistrator<Algorithm> AlgorithmVERTEXCOVER_APX2::AlgorithmVertexCoverAPX2Registrator(
            &Algorithm::AlgorithmFactory, "vertexcover", new DefaultInstantiator<Algorithm, AlgorithmVERTEXCOVER_APX2>(
            "vertexcover", "Adds an vertex cover approximation to the graph", "http://en.wikipedia.org/wiki/Vertex_cover"));

        void AlgorithmVERTEXCOVER_APX2::FindVertexCover(Graph& G, VertexSet& VertexCover)
        {
            VertexCover.clear();
            EdgeSet MaximalMatching;
            AlgorithmMAXIMALMATCHING::FindMaximalMatching(G,MaximalMatching);
            for(EdgeIterator e = MaximalMatching.begin(); e != MaximalMatching.end(); e++)
            {
                VertexSet Incident = (**e).CollectIncidentVertices(1,1,1);
                VertexCover += Incident;
            }
        }

        void AlgorithmVERTEXCOVER_APX2::AddVertexCover(Graph &G, string VertexCoverName)
        {
            VertexSet VertexCover;
            FindVertexCover(G, VertexCover);
            G.AddVertexSet(VertexCover, VertexCoverName);
        }

        void AlgorithmVERTEXCOVER_APX2::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string VertexCoverName =  parameters[0];
            AddVertexCover(G, VertexCoverName);
        }

        bool AlgorithmVERTEXCOVER_APX2::CanGuaranteeApproximationDistance(const Graph& G, float MaxApproximationDistance)
        {
            return MaxApproximationDistance <= 2.0f;
        }

    }
}
