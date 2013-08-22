
#ifndef __OPENGT_EXPORT_EXPORT_H
    #define __OPENGT_EXPORT_EXPORT_H

    #include <string>
    #include <ostream>
    #include "../factory.h"
    #include "../opengt.h"
    #include "../visualize/color.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilter
            {
                public:
                    static Factory<ExportFilter> ExportFilterFactory;
                    virtual ~ExportFilter();

                    virtual void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                std::map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                std::map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi=72,
                                float edgewidth=-1, float vertexradius=-1) = 0;
                    void DoExport(OpenGraphtheory::Graph& G, std::ostream& os, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    void DoExport(OpenGraphtheory::Graph& G, std::string FileName, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    std::string DoExport(OpenGraphtheory::Graph& G, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    static void Export(OpenGraphtheory::Graph& G, std::ostream& os, std::string format, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    static void Export(OpenGraphtheory::Graph& G, std::string FileName, std::string format, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72,float edgewidth=-1, float vertexradius=-1);


            };

            class GraphicalExportFilter : public ExportFilter
            {
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                std::map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                std::map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi=72,
                                float edgewidth=-1, float vertexradius=-1);

                    virtual void Begin(std::ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI);
                    virtual void End(std::ostream &os);
                    virtual void SetPenColor(std::ostream &os, Visualization::Color);
                    virtual void SetBrushColor(std::ostream &os, Visualization::Color);
                    virtual void SetLineWidth(std::ostream &os, float Width);

                    virtual void BeginDeclaringVertices(std::ostream &os);
                    virtual void DeclareVertex(std::ostream &os, int vertex_id, float x, float y, float radius, std::string text);
                    virtual void EndDeclaringVertices(std::ostream &os);

                    virtual void BeginRenderingEdges(std::ostream &os);
                    virtual void Line(std::ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    virtual void Arrow(std::ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    virtual void EndRenderingEdges(std::ostream &os);

                    virtual void BeginRenderingVertices(std::ostream &os);
                    virtual void Circle(std::ostream &os, int node_id, float x, float y, float radius);
                    virtual void EndRenderingVertices(std::ostream &os);

                    virtual void PutText(std::ostream &os, float x, float y, std::string text);
                    virtual void PutVertexText(std::ostream &os, int node_id, float x, float y, std::string text);
                    virtual void PutEdgeText(std::ostream &os, int from_id, int to_id, float x, float y, std::string text);
            };

        }
    }

#endif
