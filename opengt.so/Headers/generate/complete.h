#ifndef __OPENGRAPHTHEORY_GENERATE_COMPLETE_H
    #define __OPENGRAPHTHEORY_GENERATE_COMPLETE_H

    #include <cmath>
    #include <list>
    #include "../opengt.h"
    #include "../factory.h"
    #include "generate.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            /// \ingroup Generate
            /// \brief Generator Class for <a href="http://en.wikipedia.org/wiki/Complete_graph">Complete Graphs</a>

            class GeneratorCOMPLETE : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorCompleteRegistrator;
                public:
                    GeneratorCOMPLETE();
                    Graph DoGenerate(std::list<int> parameter);
            };

        }
    }

#endif
