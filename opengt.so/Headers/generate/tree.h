#ifndef __OPENGRAPHTHEORY_GENERATE_TREE_H
    #define __OPENGRAPHTHEORY_GENERATE_TREE_H

    #include <cmath>
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
                    void InternalGenerate(Graph* G, std::list<int>* parameter, std::list<int>::iterator* it, VertexIterator v);
                public:
                    GeneratorTREE();
                    Graph DoGenerate(std::list<int> parameter);
            };

        }
    }

#endif
