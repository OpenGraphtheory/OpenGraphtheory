
#ifndef __OPENGRAPHTHEORY_LOGIC_COMPUTATIONTREELOGIC_CTL_LEXER_H
    #define __OPENGRAPHTHEORY_LOGIC_COMPUTATIONTREELOGIC_CTL_LEXER_H

    #ifndef yyFlexLexerOnce
        #include<FlexLexer.h>
    #endif


    #undef YY_DECL
    #define YY_DECL int OpenGraphtheory::Logic::CTLLexer::yylex()
    #include "../../../Sources/logic/ComputationTreeLogic/ctl_parser.hpp"

    namespace OpenGraphtheory
    {
        namespace Logic
        {


            class CTLLexer : public yyFlexLexer
            {
                private:
                    CTLParser::semantic_type * yylval;
                    int yylex();
                public:
                    int yylex(CTLParser::semantic_type * lval);
                    CTLLexer(istream& is);
            };


        }
    }


#endif
