%{
#include "../../../Headers/import/dot/dot_lexer.h"
#define yyterminate() return DOTParser::token::END
%}

%option c++
%option case-insensitive
%option yyclass="OpenGraphtheory::Import::DOTLexer"
%option full
%option noyywrap
%option nounput
%option batch
%option prefix="Import_DOT_"

%%

[\ \n\t\r]+                          {}
"//"[^\n]*\n                         {}
"graph"                              { return DOTParser::token::GRAPH; }
"["                                  { return DOTParser::token::OPENBRACKET; }
"]"                                  { return DOTParser::token::CLOSEBRACKET; }
"{"                                  { return DOTParser::token::OPENBRACES; }
"}"                                  { return DOTParser::token::CLOSEBRACES; }
";"                                  { return DOTParser::token::SEMICOLON; }
","                                  { return DOTParser::token::COMMA; }
"="                                  { return DOTParser::token::EQ; }

"--"|"->"                            { return DOTParser::token::TO; }


[a-zA-Z][0-9a-zA-Z]*                 { (*yylval).sval = new(std::string)(yytext); return DOTParser::token::STRING; }
\"[^\"]*\"                           { (*yylval).sval = new(std::string)(yytext+1,yyleng-2); return DOTParser::token::STRING; }
.                                    {std::cerr << "lexical error: unexpected character " << yytext << std::endl; }

%%

DOTLexer::DOTLexer(std::istream& is) : Import_DOT_FlexLexer(&is)
{
}

int DOTLexer::yylex(DOTParser::semantic_type* lval)
{
    yylval = lval;
    return yylex();
}

