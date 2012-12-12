
#ifndef __OPENGRAPHTHEORY_LOGIC_FIRSTORDERLOGIC_FO_LEXER_H
    #define __OPENGRAPHTHEORY_LOGIC_FIRSTORDERLOGIC_FO_LEXER_H

    #ifndef yyFlexLexerOnce
        #define yyFlexLexer Logic_FO_FlexLexer
        #include<FlexLexer.h>
    #endif


    #undef YY_DECL
    #define YY_DECL int OpenGraphtheory::Logic::FOLexer::yylex()
    #include "../../../Sources/logic/FirstOrderLogic/fo_parser.hpp"

    namespace OpenGraphtheory
    {
        namespace Logic
        {


            class FOLexer : public Logic_FO_FlexLexer
            {
                private:
                    FOParser::semantic_type * yylval;
                    int yylex();
                public:
                    int yylex(FOParser::semantic_type * lval);
                    FOLexer(istream& is);
            };


        }
    }


#endif
