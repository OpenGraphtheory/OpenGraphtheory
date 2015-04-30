
#include "../../../Headers/algorithms/vertexseparator/vertexseparator.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        MultiFactoryRegistrator<Algorithm> AlgorithmVERTEXSEPARATOR::AlgorithmVertexSeparatorRegistrator(
            &Algorithm::AlgorithmFactory, "vertexseparator", new DefaultInstantiator<Algorithm, AlgorithmVERTEXSEPARATOR>(
            "vertexseparator", "adds a minimum vertex-separator to the graph", "http://en.wikipedia.org/wiki/Vertex_separator"));


        bool AlgorithmVERTEXSEPARATOR::FindMinimumVertexSeparator(Graph &G, VertexSet& Sources, VertexSet& Drains, VertexSet& Separator)
        {
            EdgeSet DisjointPaths;
            return FindDisjointPathsAndSeparator(G, Sources, Drains, DisjointPaths, Separator);
        }

        bool AlgorithmVERTEXSEPARATOR::FindMinimumVertexSeparator(Graph &G, Vertex* Source, Vertex* Drain, VertexSet& Separator)
        {
            VertexSet Sources;
            Sources.insert(Source);
            VertexSet Drains;
            Drains.insert(Drain);
            return FindMinimumVertexSeparator(G, Sources, Drains, Separator);
        }

        void AlgorithmVERTEXSEPARATOR::AddMinimumVertexSeparator(Graph &G, VertexSet& Sources, VertexSet& Drains, string SeparatorName)
        {
            VertexSet Separator;
            if(FindMinimumVertexSeparator(G, Sources, Drains, Separator))
            {
                G.AddVertexSet(Separator, SeparatorName);
            }
        }

        void AlgorithmVERTEXSEPARATOR::AddMinimumVertexSeparator(Graph &G, Vertex* Source, Vertex* Drain, string SeparatorName)
        {
            VertexSet Sources;
            Sources.insert(Source);
            VertexSet Drains;
            Drains.insert(Drain);
            AddMinimumVertexSeparator(G, Sources, Drains, SeparatorName);
        }

        void AlgorithmVERTEXSEPARATOR::Run(Graph& G, std::vector<std::string> parameters)
        {
            if(parameters.size() < 3)
                throw "vertex separator algorithm needs 3 parameters (source, drain, result name)";

            string SourceName = parameters[0];
            string DrainName = parameters[1];
            string SeparatorName = parameters[2];

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
                throw "vertex separator algorithm: source not found";
            if(Drain == G.EndVertices())
                throw "vertex separator algorithm: drain not found";

            AddMinimumVertexSeparator(G, *Source, *Drain, SeparatorName);
        }

    }
}
