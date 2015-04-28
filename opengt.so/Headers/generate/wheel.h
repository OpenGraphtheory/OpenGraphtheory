#ifndef __OPENGRAPHTHEORY_GENERATE_WHEEL_H
    #define __OPENGRAPHTHEORY_GENERATE_WHEEL_H

    #include <list>
    #include <cmath>

    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            /// \ingroup Generate
            /// \brief Generator Class for <a href="http://en.wikipedia.org/wiki/Wheel_graph">Wheel Graphs</a>

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
