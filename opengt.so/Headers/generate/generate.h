
#ifndef __OPENGRAPHTHEORY_GENERATE_GENERATE_H
    #define __OPENGRAPHTHEORY_GENERATE_GENERATE_H

    #include <list>
    #include <string>
    #include "../opengt.h"
    #include "../factory.h"

    namespace OpenGraphtheory
    {
        namespace Generate
        {

            class Generator
            {
                protected:
                    int MinParamCount;
                    int MaxParamCount;

                public:
                    static Factory<Generator> GeneratorFactory;

                    Generator();
                    Generator(int MinParamCount, int MaxParamCount);
                    virtual ~Generator();

                protected:
                    virtual OpenGraphtheory::Graph DoGenerate(std::list<int> parameter) = 0;
                public:
                    Graph Generate(list<int> parameter);
                    static OpenGraphtheory::Graph Generate(std::string generator, std::list<int> parameter);
            };

        }
    }

#endif
