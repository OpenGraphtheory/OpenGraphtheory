#ifndef __OPENGRAPHTHEORY_EXPORT_TIKZ_H
    #define __OPENGRAPHTHEORY_EXPORT_TIKZ_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterTIKZ : public GraphicalExportFilter
            {
                protected:

                    static FactoryRegistrator<ExportFilter> ExportFilterTikzRegistrator;
                    float ImageHeight;
                    float LineWidth;
                    bool RenderingVertices;
                    bool RenderingEdges;

                    std::map<char, std::string> SpecialCharacters();
                    std::string SpecialCharacter_auml();
                    std::string SpecialCharacter_Auml();
                    std::string SpecialCharacter_ouml();
                    std::string SpecialCharacter_Ouml();
                    std::string SpecialCharacter_uuml();
                    std::string SpecialCharacter_Uuml();
                    std::string SpecialCharacter_szlig();

                public:
                    void Begin(std::ostream &os, float WidthInCm, float HeightInCm, float ResolutionDPI);
                    void End(std::ostream &os);
                    void SetPenColor(std::ostream &os, Visualization::Color);
                    void SetBrushColor(std::ostream &os, Visualization::Color);
                    void SetLineWidth(std::ostream &os, float Width);
                    void DeclareVertex(std::ostream &os, int vertex_id, float x, float y, float radius, std::string text);
                    void BeginRenderingVertices(std::ostream &os);
                    void Circle(std::ostream &os, int node_id, float x, float y, float radius);
                    void EndRenderingVertices(std::ostream &os);
                    void BeginRenderingEdges(std::ostream& os);
                    void Line(std::ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2);
                    void Arrow(std::ostream &os, int from_id, int to_id, float x1, float y1, float x2, float y2, float to_radius);
                    void EndRenderingEdges(std::ostream& os);
                    void PutText(std::ostream &os, float x, float y, std::string text);
            };

        }
    }

#endif
