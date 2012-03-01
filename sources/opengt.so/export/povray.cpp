
#include <iostream>
#include <math.h>
#include "../opengt.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportToPOVRAY(Graph& G, ostream& os)
        {
            if(G.IsHypergraph())
                throw "The POV-Ray fileformat doesn\'t support hypergraphs\n";

            os << "// www.Open-Graphtheory.org\n";
            os << "#include \"colors.inc\"\n\n";

            os << "global_settings\n";
            os << "{\n";
            os << "    assumed_gamma 1.0\n";
            os << "}\n";
            os << "camera\n";
            os << "{\n";
            os << "    location <0,0.5,-4>\n";
            os << "    look_at  <0,0,0>\n";
            os << "}\n";
            os << "sky_sphere\n";
            os << "{\n";
            os << "    pigment\n";
            os << "    {\n";
            os << "        gradient y\n";
            os << "        color_map\n";
            os << "        {\n";
            os << "            [0.0 rgb <0.6,0.7,1.0>]\n";
            os << "            [0.7 rgb <0.0,0.1,0.8>]\n";
            os << "        }\n";
            os << "    }\n";
            os << "}\n";
            os << "light_source\n";
            os << "{\n";
            os << "    <-30,30,-30>\n";
            os << "    color rgb <1,1,1>\n";
            os << "}\n";

            // spheres for vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                os << "// v" << v.GetID() << "\n";
                os << "sphere\n";
                os << "{\n";
                os << "    <" << v.GetX() << "," << v.GetY() << "," << v.GetZ() << ">, 1\n";
                os << "    texture\n";
                os << "    {\n";
                os << "        pigment\n";
                os << "        {\n";
                os << "            color Red\n";
                os << "        }\n";
                os << "    }\n";
                os << "}\n";
            }

            // cylinders for edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                os << "// e" << e.GetID() << "\n";
                os << "cylinder\n";
                os << "{\n";
                os << "    <" << e.From().GetX() << "," << e.From().GetY() << "," << e.From().GetZ() << ">,";
                os <<     "<" << e.To().GetX() << "," << e.To().GetY() << "," << e.To().GetZ() << ">,1\n";
                os << "    pigment";
                os << "    {\n";
                os << "        color Black\n";
                os << "    }\n";
                os << "}\n";
            }

        }

    }
}
