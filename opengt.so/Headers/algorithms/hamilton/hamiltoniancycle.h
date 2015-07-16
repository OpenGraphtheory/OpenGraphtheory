
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_HAMILTON_HAMILTONIANCYCLE_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_HAMILTON_HAMILTONIANCYCLE_H

    #include "../kcycle/kcycle.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmHAMILTONIANCYCLE : public AlgorithmKCYCLE
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    bool FindHamiltonianCycle(Graph& G, EdgeSet& HamiltonianCycle);
                    void AddHamiltonianCycle(Graph &G, std::string HamiltonianCycleName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmHamiltonianCycleRegistrator;
            };
        }
    }

#endif


