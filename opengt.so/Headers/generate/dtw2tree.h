#ifndef __OPENGRAPHTHEORY_GENERATE_TREE_H
    #define __OPENGRAPHTHEORY_GENERATE_TREE_H

    #include <cmath>
    #include <list>
    #include <vector>

    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            /// \ingroup Generate
            /// \brief Generator Class for directed trees with directed tree-width 2 and arbitrary DAGwidth

            class GeneratorDTW2TREE : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorDtw2TreeRegistrator;
                    int InternalGenerate(Graph* G, Vertex* v, std::vector<Vertex*>& predecessors, int height, int startid);
                public:
                    GeneratorDTW2TREE();
                    Graph DoGenerate(std::list<int> parameter);
            };

        }
    }

#endif
