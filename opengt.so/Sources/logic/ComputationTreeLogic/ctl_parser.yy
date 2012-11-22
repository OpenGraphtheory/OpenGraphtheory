%code requires {

    // Many thanks to
    // http://www.thebinaryidiot.com/archives/2010/03/06/flex-bison-cpp-example/

    #include<string>
    #include "../../../Headers/logic/ComputationTreeLogic/ctl.h"

    namespace OpenGraphtheory { namespace Logic { class CTLLexer; } }
    using namespace OpenGraphtheory::Logic;
}

%skeleton "lalr1.cc"
%defines
%define namespace "OpenGraphtheory::Logic"
%define parser_class_name "CTLParser"
%parse-param { OpenGraphtheory::Logic::CTLLexer &scanner }
%parse-param { OpenGraphtheory::Logic::CTLFormula** result }
%lex-param { OpenGraphtheory::Logic::CTLLexer &scanner }
%union
{
    std::string* sval;
    CTLFormula* fval;
};

%code {
    static int yylex(CTLParser::semantic_type * yylval, OpenGraphtheory::Logic::CTLLexer &scanner);
}

%start S
%token END 0 "end of file"
%token ALWAYS
%token EXISTS
%token UNTIL
%token FINALLY
%token NEXT
%token GLOBALLY
%token AND
%token OR
%token NOT
%token _TRUE
%token _FALSE
%token OPEN
%token CLOSE
%token <sval> STRING

%% // Produktionen

S:       CTLFORMULA END                       { *result = $<fval>1; }
       | ACTLFORMULA END                      { *result = $<fval>1; }
       ;
CTLFORMULA:
       | ANDS                                 { $<fval>$ = $<fval>1; }
       | ORS                                  { $<fval>$ = $<fval>1; }

       | EXISTS NEXT ACTLFORMULA              { $<fval>$ = new CTL_ExistsNext($<fval>3); }
       | EXISTS GLOBALLY ACTLFORMULA          { $<fval>$ = new CTL_ExistsGlobally($<fval>3); }
       | EXISTS FINALLY ACTLFORMULA           { $<fval>$ = new CTL_ExistsFinally($<fval>3); }
       | EXISTS ACTLFORMULA UNTIL ACTLFORMULA { $<fval>$ = new CTL_ExistsUntil($<fval>2, $<fval>4); }

       | ALWAYS NEXT ACTLFORMULA              { $<fval>$ = new CTL_AlwaysNext($<fval>3); }
       | ALWAYS GLOBALLY ACTLFORMULA          { $<fval>$ = new CTL_AlwaysGlobally($<fval>3); }
       | ALWAYS FINALLY ACTLFORMULA           { $<fval>$ = new CTL_ExistsFinally($<fval>3); }
       | ALWAYS ACTLFORMULA UNTIL ACTLFORMULA { $<fval>$ = new CTL_AlwaysUntil($<fval>2, $<fval>4); }
       ;

ANDS:    ANDS AND ACTLFORMULA                 { $<fval>$ = new CTL_And($<fval>1, $<fval>3); }
       | ACTLFORMULA AND ACTLFORMULA          { $<fval>$ = new CTL_And($<fval>1, $<fval>3); }
       ;
ORS:     ORS OR ACTLFORMULA                   { $<fval>$ = new CTL_Or($<fval>1, $<fval>3); }
       | ACTLFORMULA OR ACTLFORMULA           { $<fval>$ = new CTL_Or($<fval>1, $<fval>3); }
       ;
ACTLFORMULA: _TRUE                            { $<fval>$ = new CTL_True(); }
       | _FALSE                               { $<fval>$ = new CTL_False(); }
       | STRING                               { $<fval>$ = new CTL_Atomic($<sval>1); }
       | NOT STRING                           { $<fval>$ = new CTL_Not( new CTL_Atomic($<sval>2) ); }
       | OPEN CTLFORMULA CLOSE                { $<fval>$ = $<fval>2; }
       | NOT OPEN CTLFORMULA CLOSE            { $<fval>$ = new CTL_Not( $<fval>3 ); }
       ;

%%

#include "../../../Headers/logic/ComputationTreeLogic/ctl_lexer.h"

void CTLParser::error(const CTLParser::location_type &loc, const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
}

static int yylex(CTLParser::semantic_type * yylval, OpenGraphtheory::Logic::CTLLexer &scanner)
{
    return scanner.yylex(yylval);
}
