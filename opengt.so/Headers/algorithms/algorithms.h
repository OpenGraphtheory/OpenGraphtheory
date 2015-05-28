
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
                    void RunInThread(Graph* G, std::vector<std::string> parameters,
                                    ConditionVariable* threadFinishedSignal=NULL, Thread** FinishedThreadRegister=NULL);

                    // MaxApproximationDistance must be >= 1
                    // MinCorrectnessProbability must be <= 1   ( = 100% )
                    static void RunParallel(std::set<Algorithm*> algos, Graph& G, std::vector<std::string> parameters,
                                            float MaxApproximationDistance=1.0, float MinCorrectnessProbability=1.0);

                    virtual ~Algorithm();

                    virtual bool SuitableFor(const Graph& G);
                    // by default, all algorithms are exact
                    virtual bool CanGuaranteeApproximationDistance(const Graph& G, float MaxApproximationDistance);
                    virtual bool CanGuaranteeCorrectnessProbability(const Graph& G, float MinCorrectnessProbability);
            };



            class ApproximationAlgorithm : public Algorithm
            {
                public:
                    virtual bool CanGuaranteeApproximationDistance(const Graph& G, float MaxApproximationDistance);
            };
            class ApproximationSchema : public ApproximationAlgorithm
            {
                public:
                    virtual bool CanGuaranteeApproximationDistance(const Graph& G, float MaxApproximationDistance);
            };


            class RandomizedAlgorithm : public Algorithm
            {
                public:
                    virtual bool CanGuaranteeCorrectnessProbability(const Graph& G, float MinCorrectnessProbability);
            };
            class MonteCarloAlgorithm : public RandomizedAlgorithm
            {
                public:
                    virtual bool CanGuaranteeCorrectnessProbability(const Graph& G, float MinCorrectnessProbability);
            };
            class LasVegasAlgorithm : public RandomizedAlgorithm
            {
                public:
                    virtual bool CanGuaranteeCorrectnessProbability(const Graph& G, float MinCorrectnessProbability);
            };

        }
    }

#endif
