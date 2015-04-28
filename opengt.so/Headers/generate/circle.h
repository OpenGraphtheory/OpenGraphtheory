#ifndef __OPENGRAPHTHEORY_GENERATE_CIRCLE_H
    #define __OPENGRAPHTHEORY_GENERATE_CIRCLE_H

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
            /// \brief Generator Class for <a href="http://en.wikipedia.org/wiki/Cycle_graph">Circle Graphs</a>

            class GeneratorCIRCLE : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorCircleRegistrator;
                public:
                    GeneratorCIRCLE();
                    Graph DoGenerate(std::list<int> parameter);
            };

        }
    }

#endif
