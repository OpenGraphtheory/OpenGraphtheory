
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_ODDCIRCLE_ODDCIRCLE_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_ODDCIRCLE_ODDCIRCLE_H

    #include "../algorithm.h"
    #include<string>
    #include<map>
    #include<list>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmODDCIRCLE : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    void AddOddCircle(Graph &G, std::string OddCircleName);
                    list<Graph::VertexIterator> FindOddCircle(Graph &G);
                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmOddCircleRegistrator;
            };
        }
    }

#endif

