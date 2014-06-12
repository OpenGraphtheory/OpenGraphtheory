#include "../../Headers/export/tikz.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Export
    {

        GraphRendererTIKZ::GraphRendererTIKZ(ostream& os)
            : GraphRenderingContextStream(os)
        {
            Translator = new StringTranslatorLatex;
        }

        bool GraphRendererTIKZ::MirroredYAxis()
        {
            return true;
        }

        void GraphRendererTIKZ::BeginRenderingGraph(float WidthInCm, float HeightInCm, float ResolutionDPI)
        {
            // (*os) << "% www.Open-Graphtheory.org\n";
            (*os) << "%\\documentclass[a4paper,10pt]{article}\n";
            (*os) << "%\\usepackage{tikz}\n";
            (*os) << "%\\begin{document}\n";
            (*os) << "\\begin{tikzpicture}\n";
            ImageHeight = HeightInCm;
            RenderingVertices = false;
            RenderingEdges = false;
        }

        void GraphRendererTIKZ::EndRenderingGraph()
        {
            (*os) << "\\end{tikzpicture}\n";
            (*os) << "%\\end{document}\n";
        }

        void GraphRendererTIKZ::DeclareVertex(int vertex_id, float x, float y, float radius, string text, Color color)
        {
            (*os) << "  \\node[fill,brushcolor,circle,minimum size=" << 2*radius << "cm] (n" << vertex_id
                  << ") at (" << x << "cm, " << ImageHeight-y << "cm){" << text << "};\n";
        }

        void GraphRendererTIKZ::SetPenColor(Visualization::Color color)
        {
            if(RenderingEdges)
                (*os) << "  \\definecolor{pencolor}{rgb}{" << (color.Red/256.0f) << ","<< (color.Green/256.0f) << "," << (color.Blue/256.0f) << "}\n";
        }

        void GraphRendererTIKZ::SetBrushColor(Visualization::Color color)
        {
            if(!RenderingVertices)
                (*os) << "  \\definecolor{brushcolor}{rgb}{" << (color.Red/256.0f) << ","<< (color.Green/256.0f) << "," << (color.Blue/256.0f) << "}\n";
        }

        void GraphRendererTIKZ::SetLineWidth(float width)
        {
            LineWidth = width;
        }


        void GraphRendererTIKZ::BeginRenderingVertices()
        {
            RenderingVertices = true;
        }

        void GraphRendererTIKZ::EndRenderingVertices()
        {
            RenderingVertices = false;
        }



        void GraphRendererTIKZ::BeginRenderingEdges()
        {
            RenderingEdges = true;
        }

        void GraphRendererTIKZ::RenderEdge(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, std::string text, Color color)
        {
            (*os) << "  \\draw[-,pencolor,line width="<<LineWidth<<"cm] (n" << from_id << ") -- (n" << to_id << ");\n";
        }

        void GraphRendererTIKZ::RenderArc(int from_id, int to_id, float x1, float y1, float x2, float y2, float width, std::string text, Color color)
        {
            (*os) << "  \\draw[->,pencolor,line width=" << LineWidth << "cm] (n" << from_id << ") -- (n" << to_id << ");\n";
        }

        void GraphRendererTIKZ::EndRenderingEdges()
        {
            RenderingEdges = false;
        }


        void GraphRendererTIKZ::PutText(float x, float y, string text)
        {
            //...
        }

        FactoryRegistrator<ExportFilter> GraphRendererTIKZ::ExportFilterTikzRegistrator(&ExportFilter::ExportFilterFactory, "tikz",
            new DefaultInstantiator<ExportFilter, GraphicalExportFilter<GraphRendererTIKZ> >("tikz", "TikZ graphics for LaTeX", "http://en.wikipedia.org/wiki/PGF/TikZ"));

    }
}
