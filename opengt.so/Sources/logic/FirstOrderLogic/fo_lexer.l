%{
#include "../../../Headers/logic/FirstOrderLogic/fo_lexer.h"
#define yyterminate() return FOParser::token::END
%}

%option c++
%option yyclass="OpenGraphtheory::Logic::FOLexer"
%option full
%option noyywrap
%option nounput
%option batch
%option prefix="Logic_FO_"

%%

[\ \n\t\r]+                 {}
"\\forall"                  {return FOParser::token::FORALL;}
"\\exists"                  {return FOParser::token::EXISTS;}

"\\wedge"                   {return FOParser::token::AND;}
"\\vee"                     {return FOParser::token::OR;}
"\\neg"                     {return FOParser::token::NOT;}

"\\top"                     {return FOParser::token::_TRUE;}
"\\bot"                     {return FOParser::token::_FALSE;}
"="                         {return FOParser::token::EQ;}
"\\neq"                     {return FOParser::token::NEQ;}

"("                         {return FOParser::token::OPEN;}
")"                         {return FOParser::token::CLOSE;}
","                         {return FOParser::token::COMMA;}
"."                         {return FOParser::token::DOT;}

[a-zA-Z][0-9a-zA-Z]*        {(*yylval).sval = new(std::string)(yytext); return FOParser::token::STRING;}
\"[a-zA-Z][0-9a-zA-Z]*\"    {(*yylval).sval = new(std::string)(yytext+1,yyleng-2); return FOParser::token::STRING;}
.                           {}

%%

FOLexer::FOLexer(std::istream& is) : Logic_FO_FlexLexer(&is)
{
}

int FOLexer::yylex(FOParser::semantic_type* lval)
{
    yylval = lval;
    return yylex();
}

