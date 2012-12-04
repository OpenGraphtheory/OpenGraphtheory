%{
#include "../../../Headers/logic/ModalLogic/ml_lexer.h"
#define yyterminate() return MLParser::token::END
%}

%option c++
%option yyclass="OpenGraphtheory::Logic::MLLexer"
%option full
%option noyywrap
%option nounput
%option batch
%option prefix="Logic_ML_"

%%

[\ \n\t\r]+                 {}
"\\Diamond"                 {return MLParser::token::DIAMOND;}
"\\Box"                     {return MLParser::token::BOX;}

"\\wedge"                   {return MLParser::token::AND;}
"\\vee"                     {return MLParser::token::OR;}
"\\neg"                     {return MLParser::token::NOT;}

"\\top"                     {return MLParser::token::_TRUE;}
"\\bot"                     {return MLParser::token::_FALSE;}

"("                         {return MLParser::token::OPEN;}
")"                         {return MLParser::token::CLOSE;}

[a-zA-Z][0-9a-zA-Z]*        {(*yylval).sval = new(string)(yytext); return MLParser::token::STRING;}
\"[a-zA-Z][0-9a-zA-Z]*\"    {(*yylval).sval = new(string)(yytext+1,yyleng-2); return MLParser::token::STRING;}
.                           {}

%%

MLLexer::MLLexer(istream& is) : Logic_ML_FlexLexer(&is)
{
}

int MLLexer::yylex(MLParser::semantic_type* lval)
{
    yylval = lval;
    return yylex();
}

