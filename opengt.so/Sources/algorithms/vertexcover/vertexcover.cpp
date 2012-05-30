
#include "../../../Headers/opengt.h"

using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace VertexCover
        {


            /*

            bool VertexCover(Graph& G, VertexSet& result, int max)
            {
                if(max < 1 && G.NumberOfEdges > 0)
                    return false;


                // Kernelization
                for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                {
                    int CurrentDegree = Degree(v,G);
                    if(CurrentDegree > max)
                    {
                        if(VertexCover(G-v, result, max-1))
                        {
                            result += v;
                            return true;
                        }
                        else
                            return false;
                    }

                    if(CurrentDegree == 1)
                    {
                        VertexSet Neighbors = Neighborhood(v,G);
                        if(VertexCover(G-v-Neighbors, result, max-1))
                        {
                            result += Neighbors;
                            return true;
                        }
                        else
                            return false;
                    }

                    if(CurrentDegree == 0)
                        return VertexCover(G-v, result, max);
                }

                if(G.NumberOfVertices() > max*(max+1)) // Kernel bigger than possible
                    return false;


                // minimize branching vector
                int MaxDegree = -1;
                VertexIterator SelectedVertex;
                for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
                {
                    int CurrentDegree = Degree(v,G);
                    if(CurrentDegree > MaxDegree)
                    {
                        MaxDegree = CurrentDegree;
                        SelectedVertex = v;
                    }
                }

                if(VertexCover(G-SelectedVertex, result, max-1))
                {
                    result += v;
                    return true;
                }

                VertexSet Neighbors = Neighbors(SelectedVertex, G);
                if(VertexCover(G-Neighbors-SelectedVertex, result, max - CurrentDegree))
                {
                    result += Neighbors;
                    return true;
                }

                return false;
            }

            VertexSet VertexCover(Graph& G)
            {
                Graph GCopy = G;
                VertexSet result;
                for(int i = 0; ; i++)
                    if(VertexCover(GCopy, result, i))
                        return result;
            }

            */


        }
    }
}
