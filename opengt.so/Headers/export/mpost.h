#ifndef __OPENGRAPHTHEORY_EXPORT_MPOST_H
    #define __OPENGRAPHTHEORY_EXPORT_MPOST_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterMPOST : public GraphicalExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterMpostRegistrator;

                    float ImageHeight;
                    Visualization::Color BrushColor;
                    Visualization::Color PenColor;

                public:

                    void Begin(ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI);
                    void End(ostream &os);
                    void SetPenColor(ostream &os, Visualization::Color);
                    void SetBrushColor(ostream &os, Visualization::Color);
                    void SetLineWidth(ostream &os, float Width);
                    void DeclareVertex(ostream &os, int vertex_id, float x, float y, float radius, string text);
                    void Line(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    void Arrow(ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    void Circle(ostream &os, int node_id, float x, float y, float radius);
                    void PutText(ostream &os, float x, float y, string text);
                    void PutVertexText(ostream &os, int node_id, float x, float y, string text);
                    void PutEdgeText(ostream &os, int from_id, int to_id, float x, float y, string text);

            };

        }
    }

#endif
