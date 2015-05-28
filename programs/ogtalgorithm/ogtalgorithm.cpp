
#include <iostream>
#include <stdlib.h>
#include <list>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/algorithms/algorithms.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

void usage(char* argv0);
int main(int argc, char** argv)
{
    set<Algorithm*> algos;
    int result = 0;
    try
    {
        Graph G;
        vector<string> parameters;
        int argvi = 2;
        cin >> G;
        float MinApproximationQuality = 1.0;

        for(; argvi < argc; argvi++)
        {
            if(string(argv[argvi]) == "--quality" && argc > argvi+1)
            {
                float temp = atof(argv[++argvi]);
                if(temp < 1.0f)
                    temp = 1.0f;
                MinApproximationQuality = 1.0f / temp;
            }
            else
            {
                parameters.push_back(string(argv[argvi]));
            }
        }


        if(argc < 2)
            throw "no algorithm selected";
        algos = Algorithm::AlgorithmFactory.Produce(argv[1]);

        if(algos.size() < 1)
            throw "no such algorithm";

        Algorithm::RunParallel(algos, G, parameters, MinApproximationQuality);
        cout << G;
        cout.flush();
    }
    catch(const char* s)
    {
        cerr << argv[0] << " ERROR: " << s << "\n";
        usage(argv[0]);
        result = 1;
    }

    while(algos.size() > 0)
    {
        delete *(algos.begin());
        algos.erase(algos.begin());
    }

    return result;
}






class FactoryMaxNameLength : public OpenGraphtheory::FactoryEnumerator
{
    unsigned int length;
    public:
        FactoryMaxNameLength();
        void Enumerate(string name, string description, string url);
        int GetLength();
};
FactoryMaxNameLength::FactoryMaxNameLength()
{
    length = 0;
}
void FactoryMaxNameLength::Enumerate(string name, string description, string url)
{
    if(name.size() > length)
        length = name.size();
}
int FactoryMaxNameLength::GetLength()
{
    return length;
}



class FactoryLister : public OpenGraphtheory::FactoryEnumerator
{
    int length;
    ostream* os;
    public:
        FactoryLister(int Col1Width, ostream& os);
        void Enumerate(string name, string description, string url);
};
FactoryLister::FactoryLister(int Col1Width, ostream &os)
{
    length = Col1Width;
    this->os = &os;
}
void FactoryLister::Enumerate(string name, string description, string url)
{
    *os << "      " << name;
    for(int i = name.length(); i < length; i++)
        *os << " ";
    *os << "  " << description << "\n      ";
    for(int i = 0; i < length; i++)
        *os << " ";
    *os << "  " << url << "\n";
}


void usage(char* argv0)
{
    cerr << "usage: " << argv0 << " algorithm <parameters>\n"
         << "     where algorithm can be\n";

    FactoryMaxNameLength* l = new FactoryMaxNameLength();
    Algorithm::AlgorithmFactory.Enumerate(l);
    FactoryLister* w = new FactoryLister(l->GetLength(), cerr);
    Algorithm::AlgorithmFactory.Enumerate(w);
    delete l;
    delete w;
}

