#ifndef __OPENGRAPHTHEORY_GENERATE_WHEEL_H
    #define __OPENGRAPHTHEORY_GENERATE_WHEEL_H

    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class GeneratorWHEEL : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorWheelRegistrator;
                public:
                    GeneratorWHEEL();
                    Graph DoGenerate(std::list<int> parameter);
            };

        }
    }

#endif
