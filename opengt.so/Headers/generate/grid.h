#ifndef __OPENGRAPHTHEORY_GENERATE_GRID_H
    #define __OPENGRAPHTHEORY_GENERATE_GRID_H

    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class GeneratorGRID : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorGridRegistrator;
                public:
                    GeneratorGRID();
                    Graph DoGenerate(list<int> parameter);
                    Graph Generate(int w, int h);
            };

        }
    }

#endif
