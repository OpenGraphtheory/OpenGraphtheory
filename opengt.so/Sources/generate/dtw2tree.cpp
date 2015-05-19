
#include "../../Headers/generate/dtw2tree.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Generate
    {

        FactoryRegistrator<Generator> GeneratorDTW2TREE::GeneratorDtw2TreeRegistrator(
            &Generator::GeneratorFactory, "dtw2tree", new DefaultInstantiator<Generator, GeneratorDTW2TREE>(
                "dtw2tree", "directed \"trees\" with directed tree-width 2 but arbitrary DAGwidth",""));

        GeneratorDTW2TREE::GeneratorDTW2TREE()
        {
            MinParamCount = 1;
            MaxParamCount = 1;
        }

        int GeneratorDTW2TREE::InternalGenerate(Graph* G, Vertex* v, vector<Vertex*>& predecessors, int height, int startid)
        {
            if(height <= 1)
            {
                for(vector<Vertex*>::iterator p = predecessors.begin(); p != predecessors.end(); p++)
                {
                    Vertex* pred = *p;
                    G->AddArc(v, pred);
                }
                return 0;
            }

            int nodescreated = 0;
            for(int i = 0; i < 2; i++)
            {
                Vertex* u = *(G->AddVertex());
                nodescreated++;

                string strid;
                stringstream s;
                s << startid+nodescreated;
                s >> strid;
                u->SetLabel(strid);

                G->AddArc(v,u);
                predecessors.push_back(v);
                nodescreated += InternalGenerate(G, u, predecessors, height-1, startid+nodescreated);
                predecessors.pop_back();
            }
            return nodescreated;
        }

        Graph GeneratorDTW2TREE::DoGenerate(list<int> parameter)
        {
            int height = *parameter.begin();

            Graph result;

            Vertex* root = *(result.AddVertex());
            root->SetLabel("0");
            vector<Vertex*> predecessors;

            InternalGenerate(&result, root, predecessors, height, 0);
            return result;
        }

    }
}

