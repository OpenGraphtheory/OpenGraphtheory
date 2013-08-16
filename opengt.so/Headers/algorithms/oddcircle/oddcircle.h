
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
                    static void AddOddCircle(Graph &G, std::string OddCircleName);
                    static bool FindOddCircle(Graph &G, std::list<Graph::VertexIterator>& OddCircle);
                protected:
                    static FactoryRegistrator<Algorithm> AlgorithmOddCircleRegistrator;
            };
        }
    }

#endif

