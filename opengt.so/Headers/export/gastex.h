#ifndef __OPENGRAPHTHEORY_EXPORT_GASTEX_H
    #define __OPENGRAPHTHEORY_EXPORT_GASTEX_H

    #include <ostream>
    #include "../opengt.h"
    #include "../factory.h"
    #include "export.h"

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class ExportFilterGASTEX : public ExportFilter
            {
                protected:
                    static FactoryRegistrator<ExportFilter> ExportFilterGastexRegistrator;

                    std::map<char, std::string> SpecialCharacters();
                    std::string SpecialCharacter_auml();
                    std::string SpecialCharacter_Auml();
                    std::string SpecialCharacter_ouml();
                    std::string SpecialCharacter_Ouml();
                    std::string SpecialCharacter_uuml();
                    std::string SpecialCharacter_Uuml();
                    std::string SpecialCharacter_szlig();

                public:
                    void Export(OpenGraphtheory::Graph& G, std::ostream& os,
                                std::map<Graph::VertexIterator, Visualization::Color>& vertexcoloring,
                                std::map<Graph::EdgeIterator, Visualization::Color>& edgecoloring, float dpi, float edgewidth, float vertexradius);
            };
        }
    }

#endif
