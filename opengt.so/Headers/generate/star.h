#ifndef __OPENGRAPHTHEORY_GENERATE_STAR_H
    #define __OPENGRAPHTHEORY_GENERATE_STAR_H

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
            /// \brief Generator Class for <a href="http://en.wikipedia.org/wiki/Star_(graph_theory)">Star Graphs</a>

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
