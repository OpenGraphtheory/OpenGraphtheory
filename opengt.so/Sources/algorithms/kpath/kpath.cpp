
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


        bool AlgorithmKPATH::DoTestKPath(Graph &G, int k, Vertex* source, Vertex* target, VertexSet& Visited, EdgeSet& KPath, bool cyclic)
        {
            if(k <= 0)
                return target==NULL || source==target;

            EdgeSet incident = source->CollectIncidentEdges(1,1,0);
            Visited.insert(source);
            for(EdgeIterator e = incident.begin(); e != incident.end(); e++)
            {
                VertexSet neighborhood = (*e)->CollectIncidentVertices(1,1,0);
                for(VertexIterator n = neighborhood.begin(); n != neighborhood.end(); n++)
                {
                    if(Visited.find(*n) != Visited.end()) // already visited
                        if(!cyclic || k>1 || *n != target) // n is allowed if last step in cycle
                            continue;
                    if(TestKPath(G, k-1, *n, target, Visited, KPath, cyclic))
                    {
                        KPath.insert(*e);
                        return true;
                    }
                }
            }
            Visited.erase(source);
            return false;
        }

        bool AlgorithmKPATH::TestKPath(Graph &G, int k, Vertex* source, Vertex* target, VertexSet& Visited, EdgeSet& KPath, bool cyclic)
        {
            if(k >= G.NumberOfVertices() + cyclic)
                return false;

            if(cyclic) // cyclic cases => enforce target=source
            {
                if(source == NULL && target != NULL)
                    source = target;

                if(source != NULL)
                    return DoTestKPath(G,k,source,source,Visited,KPath,cyclic);

                int tried = 0;
                for(VertexIterator s = G.BeginVertices(); s != G.EndVertices(); s++)
                {
                    if(DoTestKPath(G,k,*s,*s,Visited,KPath,cyclic))
                        return true;
                    tried++;

                    // at least one vertex of the cycle has to be among the first n-k
                    // vertices of G => if we have not found one yet, no one exists.
                    // also handles the case that for hamiltonian cycles, any s must work
                    if(k+tried > G.NumberOfVertices())
                        break;
                }
                return false;
            }
            else
            {
                if(source != NULL)
                    return DoTestKPath(G,k,source,target,Visited,KPath,cyclic);

                for(VertexIterator s = G.BeginVertices(); s != G.EndVertices(); s++)
                    if(DoTestKPath(G,k,*s,target,Visited,KPath, cyclic))
                        return true;
                return false;
            }
        }

        bool AlgorithmKPATH::FindKPath(Graph &G, int k, EdgeSet& KPath)
        {
            VertexSet Visited;
            return TestKPath(G, k, NULL, NULL, Visited, KPath, false);
        }


        void AlgorithmKPATH::AddKPath(Graph &G, int k, string KPathName)
        {
            if(G.NumberOfVertices() <= 0)
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

