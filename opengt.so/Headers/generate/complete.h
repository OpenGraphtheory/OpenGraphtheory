#ifndef __OPENGRAPHTHEORY_GENERATE_COMPLETE_H
    #define __OPENGRAPHTHEORY_GENERATE_COMPLETE_H

    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class GeneratorCOMPLETE : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorCompleteRegistrator;
                public:
                    GeneratorCOMPLETE();
                    Graph DoGenerate(list<int> parameter);
            };

        }
    }

#endif