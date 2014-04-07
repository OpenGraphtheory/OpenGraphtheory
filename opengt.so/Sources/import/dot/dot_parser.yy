%code requires {

    // Many thanks to
    // http://www.thebinaryidiot.com/archives/2010/03/06/flex-bison-cpp-example/

    #include "../../../Headers/import/dot/dot.h"

    namespace OpenGraphtheory { namespace Import { class DOTLexer; } }
    using namespace OpenGraphtheory::Import;
}

%skeleton "lalr1.cc"
%defines
%define namespace "OpenGraphtheory::Import"
%define parser_class_name "DOTParser"
%parse-param { OpenGraphtheory::Import::DOTLexer &scanner }
%parse-param { OpenGraphtheory::Import::DotCode** result }
%lex-param { OpenGraphtheory::Import::DOTLexer &scanner }
%union
{
    std::string* sval;
    DotCode* dval;
    DotParameter* pval;
};

%code {
    static int yylex(DOTParser::semantic_type * yylval, OpenGraphtheory::Import::DOTLexer &scanner);
}

%start S
%token END 0 "end of file"
%token GRAPH

%token OPENBRACKET
%token CLOSEBRACKET
%token OPENBRACES
%token CLOSEBRACES

%token COMMA
%token SEMICOLON
%token TO
%token EQ
%token <sval> STRING

%% // Produktionen

S:
         GRAPH STRING OPENBRACES DOTDOCUMENT CLOSEBRACES { *result = $<dval>4; }
       ;
DOTDOCUMENT:                                  { $<dval>$ = NULL; }
       | DOTDOCUMENT DOTLINE                  { $<dval>$ = $<dval>2; $<dval>$->PrevLines = $<dval>1; }
       ;

DOTLINE:
         STRING SEMICOLON                     { $<dval>$ = new DotNode($<sval>1, NULL); }
       | STRING OPENBRACKET PARAMETERS CLOSEBRACKET SEMICOLON
                                              { $<dval>$ = new DotNode($<sval>1, $<pval>3); }
       | STRING TO STRING SEMICOLON
                                              { $<dval>$ = new DotEdge($<sval>1, $<sval>3, NULL); }
       | STRING TO STRING OPENBRACKET PARAMETERS CLOSEBRACKET SEMICOLON
                                              { $<dval>$ = new DotEdge($<sval>1, $<sval>3, $<pval>5); }
       ;
PARAMETERS :
                                              { $<pval>$ = NULL; }
       | PARAMETERS PARAMETER                 { $<pval>$ = $<pval>2; $<pval>$->PrevParameters = $<pval>1;}
PARAMETER : STRING EQ STRING COMMA            { $<pval>$ = new DotParameter($<sval>1, $<sval>3); }
       | STRING EQ STRING                     { $<pval>$ = new DotParameter($<sval>1, $<sval>3); }
%%

#include "../../../Headers/import/dot/dot_lexer.h"

void DOTParser::error(const DOTParser::location_type &loc, const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
}

static int yylex(DOTParser::semantic_type * yylval, OpenGraphtheory::Import::DOTLexer &scanner)
{
    return scanner.yylex(yylval);
}
