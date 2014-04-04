#ifndef __OPENGRAPHTHEORY_GENERATE_RANDOM_H
    #define __OPENGRAPHTHEORY_GENERATE_RANDOM_H

    #include <stdlib.h>
    #include <time.h>
    #include <vector>
    #include <set>
    #include <list>

    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class GeneratorRANDOM : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorRandomRegistrator;
                public:
                    GeneratorRANDOM();
                    Graph DoGenerate(std::list<int> parameter);
                    Graph Generate(int n, int m);
            };

        }
    }

#endif
