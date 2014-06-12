
#include "../../Headers/export/gastex.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterGASTEX::Export(Graph& G, ostream& os, VertexColoring& vertexcoloring,
                   EdgeColoring& edgecoloring, float dpi, float edgewidth, float vertexradius)
        {
            if(G.IsHypergraph())
                throw "The GasTeX fileformat doesn\'t support hypergraphs\n";

            StringTranslatorLatex Translator;

            os << "% www.Open-Graphtheory.org\n";
            os << "% INCOMPATIBLE TO PDFLATEX!!! MUST USE PLAIN LATEX (DVI)\n";
            os << "%\\documentclass[a4paper,10pt]{article}\n";
            os << "%\\usepackage{gastex}\n";
            os << "%\\begin{document}\n";
            os << "\\begin{picture}(100,100)(0,0)\n";

            /// write vertices
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                Coordinates coordinates = (*v)->GetCoordinates(2);
                os << "  \\node(n" << (*v)->GetID() << ")(" << coordinates[0] << "," << coordinates[1] << "){" << Translator.Translate((*v)->GetLabel()) << "}\n";
            }

            /// write edges
            for(EdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
                os << "  \\drawedge" << ((*e)->IsEdge()?"[AHnb=0]":"")
                   << "(n" << (*e)->From()->GetID() << ",n" << (*e)->To()->GetID() << "){" << Translator.Translate((*e)->GetLabel()) << "}\n";

            os << "\\end{picture}\n";
            os << "%\\end{document}\n";
        }

        FactoryRegistrator<ExportFilter> ExportFilterGASTEX::ExportFilterGastexRegistrator(&ExportFilter::ExportFilterFactory, "gastex",
            new DefaultInstantiator<ExportFilter, ExportFilterGASTEX>("gastex", "LaTeX package \"GasTeX\"", "http://www.lsv.ens-cachan.fr/~gastin/gastex/"));

    }
}
