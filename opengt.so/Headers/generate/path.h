#ifndef __OPENGRAPHTHEORY_GENERATE_PATH_H
    #define __OPENGRAPHTHEORY_GENERATE_PATH_H

    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class GeneratorPATH : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorPathRegistrator;
                public:
                    GeneratorPATH();
                    Graph DoGenerate(list<int> parameter);
                    Graph Generate(int n);
            };

        }
    }

#endif
