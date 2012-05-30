#ifndef __OPENGRAPHTHEORY_GENERATE_CIRCLE_H
    #define __OPENGRAPHTHEORY_GENERATE_CIRCLE_H

    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class GeneratorCIRCLE : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorCircleRegistrator;
                public:
                    GeneratorCIRCLE();
                    Graph DoGenerate(list<int> parameter);
            };

        }
    }

#endif
