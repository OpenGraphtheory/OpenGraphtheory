
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_ALGORITHM_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_ALGORITHM_H

    #include "../opengt.h"
    #include "../factory.h"
    #include "../thread.h"
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

            class Algorithm : public Thread
            {
                protected:
                    void RunThread(void* parameter);

                public:
                    static MultiFactory<Algorithm> AlgorithmFactory;

                    virtual void Run(Graph &G, std::vector<std::string> parameters) = 0;
                    void RunInThread(const Graph& G, std::vector<std::string> parameters, ConditionVariable* threadFinishedSignal=NULL);
                    static void RunParallel(std::set<Algorithm*> algos, const Graph& G, std::vector<std::string> parameters, float MinApproximationQuality=1.0);

                    virtual ~Algorithm();

                    virtual bool SuitableFor(const Graph& G);
                    virtual float ApproximationQuality(const Graph& G);
            };

            class ExactAlgorithm : public Algorithm
            {
                float ApproximationQuality(const Graph& G);
            };

        }
    }

#endif
