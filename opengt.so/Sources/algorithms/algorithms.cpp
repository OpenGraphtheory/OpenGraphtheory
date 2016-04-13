
#include "../../Headers/algorithms/algorithms.h"

using namespace std;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactory<Algorithm> Algorithm::AlgorithmFactory;

        Algorithm::~Algorithm()
        {

        }


        void Algorithm::RunInThread(Graph *G, vector<string> parameters, ConditionVariable *threadFinishedSignal, Thread** FinishedThreadRegister)
        {
            this->Start(new pair<Graph*, vector<string> >(G, parameters), threadFinishedSignal, FinishedThreadRegister);
            // starts RunThread in a different thread
        }

        void Algorithm::RunThread(void* parameter)
        {
            pair<Graph*, vector<string> >* aparameter = (pair<Graph*, vector<string> >*)parameter;
            Graph* G = aparameter->first;
            vector<string> params = aparameter->second;
            delete aparameter;

            this->Run(*G, params);
        }



        void Algorithm::RunParallel(set<Algorithm*> algos, Graph& G, vector<string> parameters,
                                    float MaxApproximationDistance, float MinCorrectnessProbability)
        {
            set<Algorithm*> SelectedAlgorithms;
            for(set<Algorithm*>::iterator i = algos.begin(); i != algos.end(); i++)
                if((*i)->SuitableFor(G)
                && (*i)->CanGuaranteeApproximationDistance(G, MaxApproximationDistance)
                && (*i)->CanGuaranteeCorrectnessProbability(G, MinCorrectnessProbability))
                    SelectedAlgorithms.insert(*i);

            if(SelectedAlgorithms.size() == 0)
            {
                throw "No suitable algorithm found";
            }
            else if(SelectedAlgorithms.size() == 1) // we have 1 algorithm => no multithreading needed
            {
                Algorithm* algo = *SelectedAlgorithms.begin();
                algo->Run(G, parameters);
            }
            else
            {
                // we have more than 1 algorithm => run them in parallel

                // give each algorithm its own copy of G
                map<Thread*, Graph*> GraphCopies;
                for(set<Algorithm*>::iterator i = SelectedAlgorithms.begin(); i != SelectedAlgorithms.end(); i++)
                    GraphCopies[*i] = new Graph(G);


                ConditionVariable synchronize;
                Thread* finishedAlgorithm = NULL;
                synchronize.Lock();

                cerr << "starting " << SelectedAlgorithms.size() << " of " << algos.size() << " algorithms\n";
                for(set<Algorithm*>::iterator i = SelectedAlgorithms.begin(); i != SelectedAlgorithms.end(); i++)
                    (*i)->RunInThread(GraphCopies[*i], parameters, &synchronize, &finishedAlgorithm);


                while(finishedAlgorithm == NULL) // a mislead interrupt can cause the Wait to stop, therefore
                    synchronize.Wait(); // this has to be in a loop that checks whether someone has actually finished
                G = *(GraphCopies[finishedAlgorithm]);


                cerr << "someone finished. sending termination requests\n";
                for(set<Algorithm*>::iterator i = SelectedAlgorithms.begin(); i != SelectedAlgorithms.end(); i++)
                    (*i)->Terminate();
                synchronize.Unlock();

                cerr << "waiting for threads to join\n";
                for(set<Algorithm*>::iterator i = SelectedAlgorithms.begin(); i != SelectedAlgorithms.end(); i++)
                {
                    (*i)->Join();
                    delete GraphCopies[*i];
                }
                GraphCopies.clear();
                cerr << "everyone joined\n";
            }
        }


        bool Algorithm::SuitableFor(const Graph& G)
        {
            return true;
        }
        bool Algorithm::CanGuaranteeApproximationDistance(const Graph& G, float MaxApproximationDistance)
        {
            return true; // by default, all algorithms are exact
        }
        bool Algorithm::CanGuaranteeCorrectnessProbability(const Graph& G, float MinCorrectnessProbability)
        {
            return true; // by default, all algorithms are exact
        }



        bool ApproximationAlgorithm::CanGuaranteeApproximationDistance(const Graph& G, float MaxApproximationDistance)
        {
            return false;
        }
        bool ApproximationSchema::CanGuaranteeApproximationDistance(const Graph& G, float MaxApproximationDistance)
        {
            // approximation schemas can reach any quality
            return true;
        }



        bool RandomizedAlgorithm::CanGuaranteeCorrectnessProbability(const Graph& G, float MinCorrectnessProbability)
        {
            return false;
        }
        bool MonteCarloAlgorithm::CanGuaranteeCorrectnessProbability(const Graph& G, float MinCorrectnessProbability)
        {
            // MC-Algorithms can reach any probability (by repeated application)
            return true;
        }
        bool LasVegasAlgorithm::CanGuaranteeCorrectnessProbability(const Graph& G, float MinCorrectnessProbability)
        {
            // Las Vegas Algorithms are exact (only the running time is random)
            return true;
        }




    }
}
