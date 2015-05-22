
#ifndef __OPENGRAPHTHEORY_IMPORT_DOT_DOT_LEXER_H
    #define __OPENGRAPHTHEORY_IMPORT_DOT_DOT_LEXER_H

    #ifndef yyFlexLexerOnce
        #define yyFlexLexer Import_DOT_FlexLexer
        #include<FlexLexer.h>
    #endif


    #undef YY_DECL
    #define YY_DECL int OpenGraphtheory::Import::DOTLexer::yylex()
    #include "../../../Sources/import/dot/dot_parser.hpp"

    namespace OpenGraphtheory
    {
        namespace Import
        {


            class DOTLexer : public Import_DOT_FlexLexer
            {
                private:
                    DOTParser::semantic_type * yylval;
                    int yylex();
                public:
                    int yylex(DOTParser::semantic_type * lval);
                    int GetCurrentLineNumber();
                    DOTLexer(std::istream& is);
            };


        }
    }


#endif
