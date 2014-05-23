
#include "../../Headers/import/vrmlgraph.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Import
    {

        FactoryRegistrator<ImportFilter> ImportFilterVRMLGRAPH::ImportFilterVRMLGRAPHRegistrator(
            &ImportFilter::ImportFilterFactory, "vrmlgraph", new DefaultInstantiator<ImportFilter, ImportFilterVRMLGRAPH>(
                "vrmlgraph", "Virtual Reality Modeling Language", "http://vrmlgraph.i-scream.org.uk"));


        Graph ImportFilterVRMLGRAPH::Import(istream& is)
        {
            Graph result;
            map<string, VertexIterator> VertexVRMLGRAPHIDs;

            string Vertex1;
            string Vertex2;
            string Line;

            /// Read edges
            is.peek(); // update eof info
            while(!is.eof())
            {
                getline(is, Line);
                if(Line.length() <= 0)
                    continue;
                if(Line[0] == '#')
                    continue;

                stringstream strstream(Line);
                strstream >> Vertex1;
                strstream >> Vertex2;

                if(VertexVRMLGRAPHIDs.find(Vertex1) == VertexVRMLGRAPHIDs.end())
                    VertexVRMLGRAPHIDs[Vertex1] = result.AddVertex(0, 0, 0.1, Vertex1);
                if(VertexVRMLGRAPHIDs.find(Vertex2) == VertexVRMLGRAPHIDs.end())
                    VertexVRMLGRAPHIDs[Vertex2] = result.AddVertex(0, 0, 0.1, Vertex2);

                result.AddEdge(VertexVRMLGRAPHIDs[Vertex1], VertexVRMLGRAPHIDs[Vertex2]);
                is.peek(); // update eof info
            }

            return result;
        }

    }
}
