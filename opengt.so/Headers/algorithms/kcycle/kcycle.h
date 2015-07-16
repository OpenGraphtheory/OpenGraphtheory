
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_KCYCLE_KCYCLE_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_KCYCLE_KCYCLE_H

    #include "../kpath/kpath.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmKCYCLE : public AlgorithmKPATH
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    bool FindKCycle(Graph& G, int k, EdgeSet& KCycle);
                    void AddKCycle(Graph &G, int k, std::string KCycleName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmKCycleRegistrator;
            };
        }
    }

#endif


