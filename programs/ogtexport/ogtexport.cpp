
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/export/export.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Export;

void usage(char* argv0);
int main(int argc, char** argv)
{
    ExportFilter* exportfilter = NULL;
    try
    {
        string vertexcoloring = "";
        string edgecoloring = "";
        string format = "";
        float edgewidth = -1;
        float vertexradius = -1;
        float dpi = 72;
        for(int i = 1; i < argc-1; i++)
        {
            if(string(argv[i]) == "--format")
                format = argv[++i];
            else if(string(argv[i]) == "--vertexcoloring")
                vertexcoloring = argv[++i];
            else if(string(argv[i]) == "--edgecoloring")
                edgecoloring = argv[++i];
            else if(string(argv[i]) == "--dpi")
            {
                stringstream foo;
                foo << argv[++i];
                foo >> dpi;
            }
            else if(string(argv[i]) == "--edgewidth")
            {
                stringstream foo;
                foo << argv[++i];
                foo >> edgewidth;
            }
            else if(string(argv[i]) == "--vertexradius")
            {
                stringstream foo;
                foo << argv[++i];
                foo >> vertexradius;
            }
            else
            {
                cerr << "argument not understood: " << argv[i] << "\n";
            }

        }

        exportfilter = ExportFilter::ExportFilterFactory.Produce(format);
        if(exportfilter == NULL)
            throw "unknown export filter";

        Graph G;
        cin >> G;
        exportfilter->DoExport(G, cout, vertexcoloring, edgecoloring, dpi, edgewidth, vertexradius);
        delete exportfilter;
    }
    catch(const char* s)
    {
        cerr << argv[0] << " ERROR: " << s << "\n";
        usage(argv[0]);
        if(exportfilter != NULL)
            delete exportfilter;
        return 1;
    }

    return 0;
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
    cerr << "usage: " << argv0 << " --format <format> [<option>*]\n"
         << "   where <option> can be\n"
         << "       --vertexcoloring <name>\n"
         << "           selects the coloring that is used for the vertices\n"
         << "       --edgecoloring <name>\n"
         << "           selects the coloring that is used for the edges\n"
         << "       --vertexradius <float>\n"
         << "           sets a common radius for all vertices (in centimeters)\n"
         << "       --edgewidth <float>\n"
         << "           sets a common width for all edges (in centimeters)\n"
         << "       --dpi <float>\n"
         << "           sets the assumed resolution (dots per inch)\n"
         << "   and <format> can be\n";

    FactoryMaxNameLength* l = new FactoryMaxNameLength();
    Export::ExportFilter::ExportFilterFactory.Enumerate(l);
    FactoryLister* w = new FactoryLister(l->GetLength(), cerr);
    Export::ExportFilter::ExportFilterFactory.Enumerate(w);
    delete l;
    delete w;
}
