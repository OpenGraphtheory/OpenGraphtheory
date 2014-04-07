
#include "../../Headers/export/gastex.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterGASTEX::Export(Graph& G, ostream& os, map<Graph::VertexIterator, Color>& vertexcoloring, map<Graph::EdgeIterator, Color>& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            if(G.IsHypergraph())
                throw "The GasTeX fileformat doesn\'t support hypergraphs\n";

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                if(coordinates.size() < 2)
                    throw "Vertex with less than 2 coordinates found";
            }

            os << "% www.Open-Graphtheory.org\n";
            os << "% INCOMPATIBLE TO PDFLATEX!!! MUST USE LATEX (DVI)\n";
            os << "%\\documentclass[a4paper,10pt]{article}\n";
            os << "%\\usepackage{gastex}\n";
            os << "%\\begin{document}\n";
            os << "\\begin{picture}(100,100)(0,0)\n";

            /// write vertices
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                os << "  \\node(n" << v.GetID() << ")(" << coordinates[0] << "," << coordinates[1] << "){" << SanitizeString(v.GetLabel()) << "}\n";
            }

            /// write edges
            for(Graph::EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << "  \\drawedge" << (e.IsEdge()?"[AHnb=0]":"")
                   << "(n" << e.From().GetID() << ",n" << e.To().GetID() << "){" << SanitizeString(e.GetLabel()) << "}\n";

            os << "\\end{picture}\n";
            os << "%\\end{document}\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterGASTEX::ExportFilterGastexRegistrator(&ExportFilter::ExportFilterFactory, "gastex",
            new DefaultInstantiator<ExportFilter, ExportFilterGASTEX>("gastex", "LaTeX package \"GasTeX\"", "http://www.lsv.ens-cachan.fr/~gastin/gastex/"));


        map<char, string> ExportFilterGASTEX::SpecialCharacters()
        {
            map<char, string> result;
            result['{'] = "\\{";
            result['}'] = "\\}";
            result['&'] = "\\&";
            result['%'] = "\\%";
            result['$'] = "\\$";
            result['#'] = "\\#";
            result['_'] = "\\_";
            result['\\'] = "{\\textbackslash}";
            result['^'] = "{\\textasciicircum}";
            result['~'] = "{\\textasciitilde}";
            return result;
        }
        string ExportFilterGASTEX::SpecialCharacter_auml()
        {
            return "\\\"a";
        }
        string ExportFilterGASTEX::SpecialCharacter_Auml()
        {
            return "\\\"A";
        }
        string ExportFilterGASTEX::SpecialCharacter_uuml()
        {
            return "\\\"u";
        }
        string ExportFilterGASTEX::SpecialCharacter_Uuml()
        {
            return "\\\"U";
        }
        string ExportFilterGASTEX::SpecialCharacter_ouml()
        {
            return "\\\"o";
        }
        string ExportFilterGASTEX::SpecialCharacter_Ouml()
        {
            return "\\\"O";
        }
        string ExportFilterGASTEX::SpecialCharacter_szlig()
        {
            return "{\\ss}";
        }

    }
}
