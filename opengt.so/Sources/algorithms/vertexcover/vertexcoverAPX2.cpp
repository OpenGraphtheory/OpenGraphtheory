
#include "../../../Headers/algorithms/vertexcover/vertexcoverAPX2.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        FactoryRegistrator<Algorithm> AlgorithmVERTEXCOVER_APX2::AlgorithmVertexCoverAPX2Registrator(
            &Algorithm::AlgorithmFactory, "vertexcoverapx2", new DefaultInstantiator<Algorithm, AlgorithmVERTEXCOVER_APX2>(
            "vertexcoverapx2", "Adds an vertex cover approximation to the graph", "http://en.wikipedia.org/wiki/Vertex_cover"));

        void AlgorithmVERTEXCOVER_APX2::FindVertexCover(Graph& G, set<Graph::VertexIterator>& VertexCover)
        {
            VertexCover.clear();
            std::set<Graph::EdgeIterator> MaximalMatching;
            AlgorithmMAXIMALMATCHING::FindMaximalMatching(G,MaximalMatching);
            for(std::set<Graph::EdgeIterator>::iterator e = MaximalMatching.begin(); e != MaximalMatching.end(); e++)
            {
                Graph::EdgeIterator ei = *e;
                set<Graph::VertexIterator> Incident = ei.CollectIncidentVertices(1,1,1);
                SetHelper::DestructiveUnion(VertexCover, Incident);
            }
        }

        void AlgorithmVERTEXCOVER_APX2::AddVertexCover(Graph &G, string VertexCoverName)
        {
            set<Graph::VertexIterator> VertexCover;
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

    }
}
