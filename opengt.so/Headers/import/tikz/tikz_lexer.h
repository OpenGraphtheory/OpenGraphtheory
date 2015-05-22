
#ifndef __OPENGRAPHTHEORY_IMPORT_TIKZ_TIKZ_LEXER_H
    #define __OPENGRAPHTHEORY_IMPORT_TIKZ_TIKZ_LEXER_H

    #ifndef yyFlexLexerOnce
        #define yyFlexLexer Import_TIKZ_FlexLexer
        #include<FlexLexer.h>
    #endif


    #undef YY_DECL
    #define YY_DECL int OpenGraphtheory::Import::TIKZLexer::yylex()
    #include "../../../Sources/import/tikz/tikz_parser.hpp"

    namespace OpenGraphtheory
    {
        namespace Import
        {


            class TIKZLexer : public Import_TIKZ_FlexLexer
            {
                private:
                    TIKZParser::semantic_type * yylval;
                    int yylex();
                public:
                    int yylex(TIKZParser::semantic_type * lval);
                    TIKZLexer(std::istream& is);
                    int GetCurrentLineNumber();
            };


        }
    }


#endif
