
#ifndef __OPENGRAPHTHEORY_LOGIC_MODALLOGIC_ML_LEXER_H
    #define __OPENGRAPHTHEORY_LOGIC_MODALLOGIC_ML_LEXER_H

    #ifndef yyFlexLexerOnce
        #define yyFlexLexer Logic_ML_FlexLexer
        #include<FlexLexer.h>
    #endif


    #undef YY_DECL
    #define YY_DECL int OpenGraphtheory::Logic::MLLexer::yylex()
    #include "../../../Sources/logic/ModalLogic/ml_parser.hpp"

    namespace OpenGraphtheory
    {
        namespace Logic
        {


            class MLLexer : public Logic_ML_FlexLexer
            {
                private:
                    MLParser::semantic_type * yylval;
                    int yylex();
                public:
                    int yylex(MLParser::semantic_type * lval);
                    MLLexer(istream& is);
            };


        }
    }


#endif
