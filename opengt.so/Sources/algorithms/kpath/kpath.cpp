
#include "../../../Headers/algorithms/kpath/kpath.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmKPATH::AlgorithmKPathRegistrator(
            &Algorithm::AlgorithmFactory, "kpath", new DefaultInstantiator<Algorithm, AlgorithmKPATH>(
            "kpath", "Adds a k-path to the graph (if it exists)", ""));


        bool AlgorithmKPATH::TestKPath(Graph &G, int k, Vertex* source, Vertex* target, VertexSet& Visited, EdgeSet& KPath)
        {
            if(k <= 0)
                return target==NULL || source==target;

            // source == NULL && target == NULL && undirected => we could use symmetry
            if(source == NULL)
            {
                for(VertexIterator s = G.BeginVertices(); s != G.EndVertices(); s++)
                    if(TestKPath(G,k,*s,target,Visited,KPath))
                        return true;
                return false;
            }

            EdgeSet incident = source->CollectIncidentEdges(1,1,0);
            Visited.insert(source);
            for(EdgeIterator e = incident.begin(); e != incident.end(); e++)
            {
                VertexSet neighborhood = (*e)->CollectIncidentVertices(1,1,0);
                for(VertexIterator n = neighborhood.begin(); n != neighborhood.end(); n++)
                {
                    if(Visited.find(*n) != Visited.end()) // already visited
                        continue;
                    if(TestKPath(G, k-1, *n, target, Visited, KPath))
                    {
                        KPath.insert(*e);
                        return true;
                    }
                }
            }
            Visited.erase(source);
            return false;
        }


        bool AlgorithmKPATH::FindKPath(Graph &G, int k, EdgeSet& KPath)
        {
            VertexSet Visited;
            return TestKPath(G, k, NULL, NULL, Visited, KPath);
        }


        void AlgorithmKPATH::AddKPath(Graph &G, int k, string KPathName)
        {
            if(G.NumberOfVertices() <= 0 || G.NumberOfVertices() < k)
                return;

            EdgeSet KPath;
            if(FindKPath(G, k, KPath))
                G.AddEdgeSet(KPath, KPathName);
        }


        void AlgorithmKPATH::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 1)
                throw "Hamiltonian-Path Algorithm needs 2 parameters (k and name of result)";

            int k;
            stringstream s;
            s << parameters[0];
            s >> k;
            string KPathName =  parameters[1];
            AddKPath(G, k, KPathName);
        }

    }
}

