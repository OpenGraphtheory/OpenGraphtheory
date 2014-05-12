
#include "../../../Headers/algorithms/color/color.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactoryRegistrator<Algorithm> AlgorithmCOLORING::AlgorithmColoringRegistrator(
            &Algorithm::AlgorithmFactory, "color", new DefaultInstantiator<Algorithm, AlgorithmCOLORING>(
            "color", "Adds a legal (vertex-) coloring to the graph", "http://en.wikipedia.org/wiki/Graph_coloring"));

        bool AlgorithmCOLORING::CompleteColoring(Graph& G, map<Graph::VertexIterator, int>& PreColoring, int k)
        {
            Graph::VertexIterator MinChoicesVertex = G.EndVertices();
            int MaxNeighborColors = -1;
            set<int> MinChoicesUsedColors;

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(PreColoring[v] != -1) // v already has a color
                    continue;

                int UncoloredNeighbors = 0;
                set<int> used_colors;
                set<Graph::VertexIterator> neighbors = v.UnderlyingNeighborhood();
                for(set<Graph::VertexIterator>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
                {
                    if(PreColoring[*neighbor] >= 0)
                        used_colors.insert(PreColoring[*neighbor]);
                    else if(PreColoring[*neighbor] == -1)
                        UncoloredNeighbors++;
                }

                if((int)(used_colors.size()) > MaxNeighborColors)
                {
                    MaxNeighborColors = used_colors.size();
                    MinChoicesUsedColors = used_colors;
                    MinChoicesVertex = v;
                }

                if(UncoloredNeighbors < k - (int)(used_colors.size()))
                {
                    PreColoring[v] = -2;
                    if(CompleteColoring(G, PreColoring, k))
                    {
                        for(set<Graph::VertexIterator>::iterator neighbor = neighbors.begin(); neighbor != neighbors.end(); neighbor++)
                            used_colors.insert(PreColoring[*neighbor]);
                        for(int i = 0; i < k; i++)
                            if(used_colors.find(i) == used_colors.end())
                            {
                                PreColoring[v] = i;
                                return true;
                            }
                    }
                    else
                    {
                        PreColoring[v] = -1;
                        return false;
                    }
                }
            }

            if(MinChoicesVertex == G.EndVertices())
            {
                // no vertex went throught the if(PreColoring[v] != -1) test => all vertices have a color
                return true;
            }

            for(int i = 0; i < k; i++) // try all colors that are not used in the neighborhood
            {
                if(MinChoicesUsedColors.find(i) != MinChoicesUsedColors.end())
                    continue;
                PreColoring[MinChoicesVertex] = i;
                if(CompleteColoring(G, PreColoring, k))
                    return true;
            }

            PreColoring[MinChoicesVertex] = -1;
            return false;
        }


        map<Graph::VertexIterator, int> AlgorithmCOLORING::FindColoring(Graph &G)
        {
            for(int k = 1; ; k++)
            {
                map<Graph::VertexIterator, int> Colors;
                for(Graph::VertexIterator i = G.BeginVertices(); i != G.EndVertices(); i++)
                    Colors[i] = -1;

                Graph::EdgeIterator e = G.BeginEdges();
                if(e != G.EndEdges() && k >= 2)
                {
                    Colors[e.From()] = 0;
                    Colors[e.To()] = 1;
                }

                if(CompleteColoring(G, Colors, k))
                    return Colors;
            }
        }


        bool AlgorithmCOLORING::AddColoring(Graph& G, int k, string ColoringName)
        {
            map<Graph::VertexIterator, int> Colors;
            for(Graph::VertexIterator i = G.BeginVertices(); i != G.EndVertices(); i++)
                Colors[i] = -1;

            Graph::EdgeIterator e = G.BeginEdges();
            if(e != G.EndEdges() && k >= 2)
            {
                Colors[e.From()] = 1;
                Colors[e.To()] = 2;
            }

            if(CompleteColoring(G, Colors, k))
            {
                G.AddVertexColoring(Colors, ColoringName);
                return true;
            }
            else
                return false;
        }

        void AlgorithmCOLORING::AddColoring(Graph &G, string ColoringName)
        {
            G.AddVertexColoring(FindColoring(G), ColoringName);
        }

        void AlgorithmCOLORING::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string ColoringName = parameters[0];

            map<Graph::VertexIterator, int> PreColoring;
            int maxcolor = 0, mincolor = 1;

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Attribute* attr = v.Attributes().GetAttribute(ColoringName);
                if(attr == NULL)
                    continue;
                IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                if(iattr == NULL)
                    continue;

                PreColoring[v] = iattr->Value;
                if(iattr->Value > maxcolor)
                    maxcolor = iattr->Value;
                if(iattr->Value < mincolor)
                    mincolor = iattr->Value;
            }

            if(mincolor <= maxcolor) // if there is a precoloring
            {

                for(Graph::VertexIterator i = G.BeginVertices(); i != G.EndVertices(); i++)
                    if(PreColoring.find(i) != PreColoring.end())
                        PreColoring[i] -= mincolor;
                    else
                        PreColoring[i] = -1;
                maxcolor -= mincolor;

                for(int k = maxcolor; ; k++)
                {
                    if(CompleteColoring(G, PreColoring, k))
                    {
                        G.AddVertexColoring(PreColoring, ColoringName);
                        break;
                    }
                }
            }
            else
                AddColoring(G, ColoringName);
        }

    }
}
