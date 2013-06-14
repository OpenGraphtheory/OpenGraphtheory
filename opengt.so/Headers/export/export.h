
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
                                map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi=72,
                                float edgewidth=-1, float vertexradius=-1) = 0;
                    void DoExport(OpenGraphtheory::Graph& G, ostream& os, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    void DoExport(OpenGraphtheory::Graph& G, std::string FileName, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    string DoExport(OpenGraphtheory::Graph& G, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    static void Export(OpenGraphtheory::Graph& G, std::ostream& os, std::string format, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72, float edgewidth=-1, float vertexradius=-1);
                    static void Export(OpenGraphtheory::Graph& G, std::string FileName, std::string format, std::string vertexcoloring="", std::string edgecoloring="", float dpi=72,float edgewidth=-1, float vertexradius=-1);


            };

            class GraphicalExportFilter : public ExportFilter
            {
                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi=72,
                                float edgewidth=-1, float vertexradius=-1);

                    virtual void Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI);
                    virtual void End(ostream &os);
                    virtual void SetPenColor(ostream &os, Visualization::Color);
                    virtual void SetBrushColor(ostream &os, Visualization::Color);
                    virtual void SetLineWidth(ostream &os, float Width);

                    virtual void BeginDeclaringVertices(ostream &os);
                    virtual void DeclareVertex(ostream &os, int vertex_id, float x, float y, float radius, string text);
                    virtual void EndDeclaringVertices(ostream &os);

                    virtual void BeginRenderingEdges(ostream &os);
                    virtual void Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    virtual void Arrow(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    virtual void EndRenderingEdges(ostream &os);

                    virtual void BeginRenderingVertices(ostream &os);
                    virtual void Circle(ostream &os, int node_id, float x, float y, float radius);
                    virtual void EndRenderingVertices(ostream &os);

                    virtual void PutText(ostream &os, float x, float y, string text);
                    virtual void PutVertexText(ostream &os, int node_id, float x, float y, string text);
                    virtual void PutEdgeText(ostream &os, int from_id, int to_id, float x, float y, string text);
            };

        }
    }

#endif
