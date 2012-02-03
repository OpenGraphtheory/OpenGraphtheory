
#include <iostream>
#include <sstream>
#include <string>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Import
    {

        Graph ImportFromTGF(istream& is)
        {
            Graph result;
            map<int, Graph::VertexIterator> VertexTGFIDs;

            int Vertex1;
            int Vertex2;
            string Caption;

            /// Read vertices
            is.peek(); // update eof info
            while(!is.eof())
            {
                getline(is, Caption);
                if(Caption.length() <= 0)
                    continue;
                if(Caption[0] == '#')
                    break;

                stringstream strstream(Caption);
                strstream >> Vertex1;

                if(VertexTGFIDs.find(Vertex1) != VertexTGFIDs.end())
                    throw "Same vertex ID multiple times";

                while(!strstream.eof() && (strstream.peek() == ' ' || strstream.peek() == '\t' || strstream.peek() == '\r'))
                    strstream.ignore(1);
                if(strstream.eof())
                    Caption = "";
                else
                    getline(strstream, Caption);
                VertexTGFIDs[Vertex1] = result.AddVertex(-1, -1, Caption);
                is.peek(); // update eof info
            }

            /// Read edges
            is.peek(); // update eof info
            while(!is.eof())
            {
                getline(is, Caption);
                if(Caption.length() <= 0)
                    continue;
                if(Caption[0] == '#')
                    break;

                stringstream strstream(Caption);
                strstream >> Vertex1 >> Vertex2;

                if(VertexTGFIDs.find(Vertex1) == VertexTGFIDs.end()
                   || VertexTGFIDs.find(Vertex2) == VertexTGFIDs.end())
                    throw "Edge containing undeclared Vertex";

                while(!strstream.eof() && (strstream.peek() == ' ' || strstream.peek() == '\t' || strstream.peek() == '\r'))
                    strstream.ignore(1);
                if(strstream.eof())
                    Caption = "";
                else
                    getline(strstream, Caption);
                result.AddEdge(VertexTGFIDs[Vertex1], VertexTGFIDs[Vertex2], Caption);
                is.peek(); // update eof info
            }

            return result;
        }

    }
}
