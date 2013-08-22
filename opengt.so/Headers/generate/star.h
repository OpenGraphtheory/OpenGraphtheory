#ifndef __OPENGRAPHTHEORY_GENERATE_STAR_H
    #define __OPENGRAPHTHEORY_GENERATE_STAR_H

    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class GeneratorSTAR : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorStarRegistrator;
                public:
                    GeneratorSTAR();
                    Graph DoGenerate(std::list<int> parameter);
                    Graph Generate(int n);
            };

        }
    }

#endif
