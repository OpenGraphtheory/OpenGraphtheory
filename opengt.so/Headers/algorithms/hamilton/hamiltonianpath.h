
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_HAMILTON_HAMILTONIANPATH_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_HAMILTON_HAMILTONIANPATH_H

    #include "../kpath/kpath.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmHAMILTONIANPATH : public AlgorithmKPATH
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    bool FindHamiltonianPath(Graph& G, EdgeSet& HamiltonianPath);
                    void AddHamiltonianPath(Graph &G, std::string HamiltonianPathName);

                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmHamiltonianPathRegistrator;
            };
        }
    }

#endif


