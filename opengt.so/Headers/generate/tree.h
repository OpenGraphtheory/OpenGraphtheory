#ifndef __OPENGRAPHTHEORY_GENERATE_TREE_H
    #define __OPENGRAPHTHEORY_GENERATE_TREE_H

    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class GeneratorTREE : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorTreeRegistrator;
                    void InternalGenerate(Graph* G, list<int>* parameter, list<int>::iterator* it, Graph::VertexIterator v);
                public:
                    GeneratorTREE();
                    Graph DoGenerate(list<int> parameter);
            };

        }
    }

#endif
