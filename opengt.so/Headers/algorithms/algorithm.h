
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_ALGORITHM_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_ALGORITHM_H

    #include "../opengt.h"
    #include "../factory.h"
    #include <string>
    #include <vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {

            /*
            class KernelCommunication
            {
                public:
                    void FoundUpperBound(int k);
                    void FoundLowerBound(int k);
                    void FoundExactAnswer(int k);

            };
            */

            class Algorithm
            {
                public:
                    static Factory<Algorithm> AlgorithmFactory;
                    virtual void Run(Graph &G, std::vector<std::string> parameters) = 0;
                    virtual ~Algorithm();
            };

        }
    }

#endif
