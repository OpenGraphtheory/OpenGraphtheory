
#ifndef __OPENGRAPHTHEORY_IMPORT_GML_GML_LEXER_H
    #define __OPENGRAPHTHEORY_IMPORT_GML_GML_LEXER_H

    #ifndef yyFlexLexerOnce
        #define yyFlexLexer Import_GML_FlexLexer
        #include<FlexLexer.h>
    #endif

    #include <stack>
    #include <string>
    #include "gml.h"

    #undef YY_DECL
    #define YY_DECL int OpenGraphtheory::Import::GMLLexer::yylex()

    namespace OpenGraphtheory
    {
        namespace Import
        {

            class GMLLexer : public Import_GML_FlexLexer
            {
                private:
                    std::stack<GMLTreeNode*> GMLStack;
                    std::string key;
                    int yylex();
                public:
                    int yylex(GMLTreeNode* result);
                    GMLLexer(std::istream& is);
            };


        }
    }


#endif
