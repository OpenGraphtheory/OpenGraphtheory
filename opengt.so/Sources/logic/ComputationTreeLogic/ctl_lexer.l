%{
#include "../../../Headers/logic/ComputationTreeLogic/ctl_lexer.h"
#define yyterminate() return CTLParser::token::END
%}

%option c++
%option yyclass="OpenGraphtheory::Logic::CTLLexer"
%option full
%option noyywrap
%option nounput
%option batch
%option prefix="Logic_CTL_"

%%

[\ \n\t\r]+                 {}
"A"                         {return CTLParser::token::ALWAYS;}
"E"                         {return CTLParser::token::EXISTS;}
"U"                         {return CTLParser::token::UNTIL;}
"F"                         {return CTLParser::token::FINALLY;}
"X"                         {return CTLParser::token::NEXT;}
"G"                         {return CTLParser::token::GLOBALLY;}

"\\wedge"                   {return CTLParser::token::AND;}
"\\vee"                     {return CTLParser::token::OR;}
"\\neg"                     {return CTLParser::token::NOT;}

"\\top"                     {return CTLParser::token::_TRUE;}
"\\bot"                     {return CTLParser::token::_FALSE;}

"("                         {return CTLParser::token::OPEN;}
")"                         {return CTLParser::token::CLOSE;}

[a-zA-Z][0-9a-zA-Z]*        {(*yylval).sval = new(std::string)(yytext); return CTLParser::token::STRING;}
\"[a-zA-Z][0-9a-zA-Z]*\"    {(*yylval).sval = new(std::string)(yytext+1,yyleng-2); return CTLParser::token::STRING;}
.                           {}

%%

CTLLexer::CTLLexer(std::istream& is) : Logic_CTL_FlexLexer(&is)
{
}

int CTLLexer::yylex(CTLParser::semantic_type* lval)
{
    yylval = lval;
    return yylex();
}

