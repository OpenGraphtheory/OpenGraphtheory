#include "../../Headers/export/tikz.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        void ExportFilterTIKZ::Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            os << "% www.Open-Graphtheory.org\n";
            os << "%\\documentclass[a4paper,10pt]{article}\n";
            os << "%\\usepackage{tikz}\n";
            os << "%\\begin{document}\n";
            os << "\\begin{tikzpicture}\n";
            //os << "  \\tikzstyle{vertex}=[shape=circle, draw]\n";
            ImageHeight = HeightInCm;
            RenderingVertices = false;
            RenderingEdges = false;
        }

        void ExportFilterTIKZ::End(ostream &os)
        {
            os << "\\end{tikzpicture}\n";
            os << "%\\end{document}\n";
        }

        void ExportFilterTIKZ::DeclareVertex(ostream &os, int vertex_id, float x, float y, float radius, string text)
        {
                os << "  \\node (n" << vertex_id
                   << ") at (" << x << "cm, " << ImageHeight-y << "cm){" << SanitizeString(text) << "};\n";
        }

        void ExportFilterTIKZ::SetPenColor(ostream &os, Visualization::Color color)
        {
            if(RenderingEdges || RenderingVertices)
                os << "  \\definecolor{pencolor}{rgb}{" << (color.Red/256.0f) << ","<< (color.Green/256.0f) << "," << (color.Blue/256.0f) << "}\n";
        }

        void ExportFilterTIKZ::SetBrushColor(ostream &os, Visualization::Color color)
        {
            if(RenderingVertices)
                os << "  \\definecolor{brushcolor}{rgb}{" << (color.Red/256.0f) << ","<< (color.Green/256.0f) << "," << (color.Blue/256.0f) << "}\n";
        }

        void ExportFilterTIKZ::SetLineWidth(ostream &os, float width)
        {
            LineWidth = width;
        }

        void ExportFilterTIKZ::BeginRenderingVertices(ostream& os)
        {
            RenderingVertices = true;
        }

        void ExportFilterTIKZ::Circle(ostream &os, int node_id, float x, float y, float radius)
        {
            os << "  \\draw[fill=brushcolor] (n" << node_id << ") circle (" << radius << "cm);\n";
        }

        void ExportFilterTIKZ::EndRenderingVertices(ostream& os)
        {
            RenderingVertices = false;
        }


        void ExportFilterTIKZ::BeginRenderingEdges(ostream& os)
        {
            RenderingEdges = true;
        }

        void ExportFilterTIKZ::Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2)
        {
            os << "  \\draw[-,pencolor,line width="<<LineWidth<<"cm] (n" << from_id << ") -- (n" << to_id << ");\n";
        }

        void ExportFilterTIKZ::Arrow(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2, float to_radius)
        {
            os << "  \\draw[->,pencolor,line width=" << LineWidth << "cm] (n" << from_id << ") -- (n" << to_id << ");\n";
        }

        void ExportFilterTIKZ::EndRenderingEdges(ostream& os)
        {
            RenderingEdges = false;
        }


        void ExportFilterTIKZ::PutText(ostream &os, float x, float y, string text)
        {
            text = SanitizeString(text);
            //...
        }


        map<char, string> ExportFilterTIKZ::SpecialCharacters()
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
        string ExportFilterTIKZ::SpecialCharacter_auml()
        {
            return "\\\"a";
        }
        string ExportFilterTIKZ::SpecialCharacter_Auml()
        {
            return "\\\"A";
        }
        string ExportFilterTIKZ::SpecialCharacter_uuml()
        {
            return "\\\"u";
        }
        string ExportFilterTIKZ::SpecialCharacter_Uuml()
        {
            return "\\\"U";
        }
        string ExportFilterTIKZ::SpecialCharacter_ouml()
        {
            return "\\\"o";
        }
        string ExportFilterTIKZ::SpecialCharacter_Ouml()
        {
            return "\\\"O";
        }
        string ExportFilterTIKZ::SpecialCharacter_szlig()
        {
            return "{\\ss}";
        }


        FactoryRegistrator<ExportFilter> ExportFilterTIKZ::ExportFilterTikzRegistrator(&ExportFilter::ExportFilterFactory, "tikz",
            new DefaultInstantiator<ExportFilter, ExportFilterTIKZ>("tikz", "TikZ graphics for LaTeX", "http://en.wikipedia.org/wiki/PGF/TikZ"));

    }
}
