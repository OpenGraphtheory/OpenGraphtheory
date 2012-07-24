
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_INDEPENDENTSET_INDEPENDENTSET_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_INDEPENDENTSET_INDEPENDENTSET_H

    #include "../clique/clique.h"
    #include<string>
    #include<set>
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmINDEPENDENTSET : public AlgorithmCLIQUE
            {
                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmIndependentsetRegistrator;
                    static FactoryRegistrator<Algorithm> AlgorithmStablesetRegistrator;
                    void Run(Graph &G, std::vector<std::string> parameters);

                    bool FindIndependentSet(Graph& G, std::set<Graph::VertexIterator>& IndependentSet, unsigned int k);
                    void AddIndependentSet(Graph &G, std::string IndependentSetName);
            };
        }
    }

#endif


