
#include "../../../Headers/algorithms/color/color.h"
#include<map>

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        FactoryRegistrator<Algorithm> AlgorithmCOLORING::AlgorithmColoringRegistrator(
            &Algorithm::AlgorithmFactory, "coloring", new DefaultInstantiator<Algorithm, AlgorithmCOLORING>(
            "coloring", "Adds a legal (vertex-) coloring to the graph", "http://en.wikipedia.org/wiki/Graph_coloring"));

        bool AlgorithmCOLORING::Colorize(Graph& G, map<Graph::VertexIterator, int>& Colors, int k)
        {
            Graph::VertexIterator MinChoicesVertex = G.EndVertices();
            unsigned int MaxNeighborColors = 0;
            set<int> MinChoicesUsedColors;

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                if(Colors[v] != -1) // v already has a color
                    continue;

                set<int> used_colors;

                for(Graph::EdgeIterator e = v.BeginIncidentEdges(); e != v.EndIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator neighbor = e.BeginIncidentVertices(); neighbor != e.EndIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                    for(Graph::VertexIterator neighbor = e.BeginPositiveIncidentVertices(); neighbor != e.EndPositiveIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                    for(Graph::VertexIterator neighbor = e.BeginNegativeIncidentVertices(); neighbor != e.EndNegativeIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                }
                for(Graph::EdgeIterator e = v.BeginPositiveIncidentEdges(); e != v.EndPositiveIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator neighbor = e.BeginIncidentVertices(); neighbor != e.EndIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                    for(Graph::VertexIterator neighbor = e.BeginPositiveIncidentVertices(); neighbor != e.EndPositiveIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                    for(Graph::VertexIterator neighbor = e.BeginNegativeIncidentVertices(); neighbor != e.EndNegativeIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                }
                for(Graph::EdgeIterator e = v.BeginNegativeIncidentEdges(); e != v.EndNegativeIncidentEdges(); e++)
                {
                    for(Graph::VertexIterator neighbor = e.BeginIncidentVertices(); neighbor != e.EndIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                    for(Graph::VertexIterator neighbor = e.BeginPositiveIncidentVertices(); neighbor != e.EndPositiveIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                    for(Graph::VertexIterator neighbor = e.BeginNegativeIncidentVertices(); neighbor != e.EndNegativeIncidentVertices(); neighbor++)
                        used_colors.insert(Colors[neighbor]);
                }

                if(used_colors.size() > MaxNeighborColors)
                {
                    MaxNeighborColors = used_colors.size();
                    MinChoicesUsedColors = used_colors;
                    MinChoicesVertex = v;
                }
            }

            if(MinChoicesVertex == G.EndVertices())
            {
                // no vertex went throught the if(Colors[v] != -1) test => all vertices have a color
                return true;
            }

            for(int i = 1; i <= k; i++)
            {
                if(MinChoicesUsedColors.find(i) != MinChoicesUsedColors.end())
                    continue;
                Colors[MinChoicesVertex] = i;
                if(Colorize(G, Colors, k))
                    return true;
            }

            Colors[MinChoicesVertex] = -1;
            return false;
        }

        bool AlgorithmCOLORING::Colorize(Graph& G, int k, string ColorizationName)
        {
            map<Graph::VertexIterator, int> Colors;
            for(Graph::VertexIterator i = G.BeginVertices(); i != G.EndVertices(); i++)
                Colors[i] = -1;

            if(Colorize(G, Colors, k))
            {
                for(map<Graph::VertexIterator, int>::iterator i = Colors.begin(); i != Colors.end(); i++)
                {
                    Graph::VertexIterator j = i->first;
                    j.Attributes().Add(ColorizationName, "int");
                    IntAttribute* attr = dynamic_cast<IntAttribute*>(j.Attributes().GetAttribute(ColorizationName));
                    if(attr != NULL)
                        attr->Value = i->second;
                }

                return true;
            }
            else
                return false;

        }

        void AlgorithmCOLORING::Colorize(Graph &G, string ColorizationName)
        {
            for(int k = 1; ; k++)
                if(Colorize(G, k, ColorizationName))
                    return;
        }

        void AlgorithmCOLORING::Run(Graph &G, vector<string> parameters)
        {
            if(parameters.size() <= 0)
                return;

            string ColorizationName =  parameters[0];
            Colorize(G, ColorizationName);
        }

    }
}
