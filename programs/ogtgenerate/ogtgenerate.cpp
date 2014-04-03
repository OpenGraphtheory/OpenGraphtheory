
#include <iostream>
#include <iomanip>
#include <list>
#include <string>
#include <stdlib.h>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/generate/generate.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Generate;




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




int main(int argc, char** argv)
{
    try
    {

        list<int> parameters;
        for(int i = 2; i < argc; i++)
            parameters.push_back(atoi(argv[i]));

        Graph result = Generator::Generate(string(argv[1]), parameters);
        cout << result;

    }
    catch(const char* error)
    {
        cerr << "ERROR: " << error << endl;

        FactoryMaxNameLength* l = new FactoryMaxNameLength();
        Generator::GeneratorFactory.Enumerate(l);
        FactoryLister* w = new FactoryLister(l->GetLength(), cerr);
        Generator::GeneratorFactory.Enumerate(w);
        delete l;
        delete w;

        return 1;
    }

    return 0;
}
