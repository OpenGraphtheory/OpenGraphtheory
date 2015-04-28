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

            /// \ingroup Generate
            /// \brief Generator Class for <a href="http://en.wikipedia.org/wiki/Path_graph">Path Graphs</a>

            class GeneratorPATH : public Generator
            {
                protected:
                    static FactoryRegistrator<Generator> GeneratorPathRegistrator;
                public:
                    GeneratorPATH();
                    Graph DoGenerate(std::list<int> parameter);
                    Graph Generate(int n);
            };

        }
    }

#endif
