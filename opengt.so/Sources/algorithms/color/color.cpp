
#include "../../../Headers/opengt.h"
#include<map>
//#include "../../../Headers/algorithms/color/color.h"

using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace Color
        {

            bool Colorize(Graph& G, map<Graph::VertexIterator, int>& Colors, int k)
            {
            /*
                if(left <= 0)
                    return true;

                int min_choices = n+1;
                int choices;
                int colorize_vertex;
                for(int i = 0; i < n; i++)
                {
                    if(color[i] != -1)
                        continue;
                    vector<vector<bool> > used_color(n,vector<bool>(colors, false));
                    choices = colors;

                    for(int j = 0; j < n; j++)
                    {
                        if(!adjacency[i][j])
                            continue;
                        if(color[j] == -1)
                            continue;
                        if(!used_color[i][color[j]])
                        {
                            used_color[i][color[j]] = true;
                            choices--;
                        }
                    }

                    if(choices < min_choices)
                    {
                        min_choices = choices;
                        colorize_vertex = i;
                    }
                }

                for(int c = 0; c < colors; c++)
                {
                    if(!used_colors[colorize_vertex][c])
                    {
                        color[colorize_vertex] = c;
                        if(colorize(adjacency, color, colors, n, left-1))
                            return true;
                    }
                }

                Color[colorize_vertex] = -1;
            */
                return false;
            }

            bool Colorize(Graph& G, int k, string ColorizationName)
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

            void Colorize(Graph &G, string ColorizationName)
            {
                for(int k = 1; ; k++)
                    if(Colorize(G, k, ColorizationName))
                        return;
            }


        }
    }
}
