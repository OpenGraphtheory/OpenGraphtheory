%{
#include "../../../Headers/import/tikz/tikz_lexer.h"
#define yyterminate() return TIKZParser::token::END
%}

%option c++
%option case-insensitive
%option yyclass="OpenGraphtheory::Import::TIKZLexer"
%option full
%option noyywrap
%option nounput
%option batch
%option prefix="Import_TIKZ_"

%%

[\ \n\t\r]+                          {}
"%"[^\n]*\n                          {}
"\\begin{tikzpicture}"               {return TIKZParser::token::BEGINTIKZPICTURE;}
"\\end{tikzpicture}"                 {return TIKZParser::token::ENDTIKZPICTURE;}
"\\node"                             {return TIKZParser::token::NODE;}
"\\draw[fill=brushcolor]"            {return TIKZParser::token::DRAWNODE;}
"\\draw[-,pencolor,line width="      {return TIKZParser::token::DRAWEDGE;}
"\\draw[->,pencolor,line width="     {return TIKZParser::token::DRAWARC;}
"\\definecolor{pencolor}{rgb}"       {return TIKZParser::token::PENCOLOR;}
"\\definecolor{brushcolor}{rgb}"     {return TIKZParser::token::BRUSHCOLOR;}
"circle [radius="                    {return TIKZParser::token::BCIRCLE;}
"circle ("                           {return TIKZParser::token::CIRCLE;}

"cm"                                 {return TIKZParser::token::CM;}

"("                                  {return TIKZParser::token::OPEN;}
")"                                  {return TIKZParser::token::CLOSE;}
"["                                  {return TIKZParser::token::OPENBRACKET;}
"]"                                  {return TIKZParser::token::CLOSEBRACKET;}
"{"                                  {return TIKZParser::token::OPENBRACES;}
"}"                                  {return TIKZParser::token::CLOSEBRACES;}
";"                                  {return TIKZParser::token::SEMICOLON;}
","                                  {return TIKZParser::token::COMMA;}

"at"                                 {return TIKZParser::token::AT;}
"--"                                 {return TIKZParser::token::TO;}


[a-zA-Z][0-9a-zA-Z]*                 {(*yylval).sval = new(std::string)(yytext); return TIKZParser::token::STRING;}
([0-9]*\.)?[0-9]+                    {(*yylval).fval = new(float)(atof(yytext)); return TIKZParser::token::FLOAT;}
.                                    {std::cerr << "lexical error: unexpected character " << yytext << std::endl;}

%%

TIKZLexer::TIKZLexer(std::istream& is) : Import_TIKZ_FlexLexer(&is)
{
}

int TIKZLexer::yylex(TIKZParser::semantic_type* lval)
{
    yylval = lval;
    return yylex();
}

