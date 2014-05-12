
#include "../../Headers/algorithms/algorithm.h"

using namespace std;

namespace OpenGraphtheory
{
    namespace Algorithms
    {

        MultiFactory<Algorithm> Algorithm::AlgorithmFactory;

        Algorithm::~Algorithm()
        {

        }


        void Algorithm::RunInThread(const Graph &G, vector<string> parameters, ConditionVariable *threadFinishedSignal)
        {
            this->Start(new pair<Graph, vector<string> >(G, parameters), threadFinishedSignal);
            // starts RunThread in a different thread
        }

        void Algorithm::RunThread(void* parameter)
        {
            pair<Graph, vector<string> >* aparameter = (pair<Graph, vector<string> >*)parameter;
            Graph G = aparameter->first;
            vector<string> params = aparameter->second;
            delete aparameter;

            this->Run(G, params);
        }



        void Algorithm::RunParallel(set<Algorithm*> algos, const Graph& G, vector<string> parameters, float MinApproximationQuality)
        {
            ConditionVariable synchronize;
            synchronize.Lock();

            set<Algorithm*> SelectedAlgorithms;
            for(set<Algorithm*>::iterator i = algos.begin(); i != algos.end(); i++)
                //if((*i)->SuitableFor(G))
                    if((*i)->ApproximationQuality(G) >= MinApproximationQuality)
                        SelectedAlgorithms.insert(*i);

            cout << "starting " << SelectedAlgorithms.size() << " of " << algos.size() << " algorithm(s)\n";
            if(SelectedAlgorithms.size() > 0) // if this wasn't checked, noone would signal synchronize, so the wait would never end
            {
                for(set<Algorithm*>::iterator i = SelectedAlgorithms.begin(); i != SelectedAlgorithms.end(); i++)
                    (*i)->RunInThread(G, parameters, &synchronize);

                synchronize.Wait();
                cout << "someone finished. sending termination requests\n";
                for(set<Algorithm*>::iterator i = SelectedAlgorithms.begin(); i != SelectedAlgorithms.end(); i++)
                    (*i)->Terminate();
                synchronize.Unlock();

                cout << "waiting for threads to join\n";
                for(set<Algorithm*>::iterator i = SelectedAlgorithms.begin(); i != SelectedAlgorithms.end(); i++)
                    (*i)->Join();
                cout << "everyone joined\n";
            }
        }


        bool Algorithm::SuitableFor(const Graph& G)
        {
            return false;
        }

        float Algorithm::ApproximationQuality(const Graph& G)
        {
            return 0.0f;
        }

        float ExactAlgorithm::ApproximationQuality(const Graph& G)
        {
            return 1.0f;
        }

    }
}
