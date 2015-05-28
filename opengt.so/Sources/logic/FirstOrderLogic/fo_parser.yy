%code requires {

    // Many thanks to
    // http://www.thebinaryidiot.com/archives/2010/03/06/flex-bison-cpp-example/

    #include "../../../Headers/logic/FirstOrderLogic/fo.h"

    namespace OpenGraphtheory { namespace Logic { class FOLexer; } }
    using namespace OpenGraphtheory::Logic;
}

%skeleton "lalr1.cc"
%defines
%define api.namespace {OpenGraphtheory::Logic}
%define parser_class_name {FOParser}
%parse-param { OpenGraphtheory::Logic::FOLexer &scanner }
%parse-param { OpenGraphtheory::Logic::FOFormula** result }
%lex-param { OpenGraphtheory::Logic::FOLexer &scanner }
%union
{
    std::string* sval;
    FOFormula* fval;
    FO_Parameter* pval;
};

%code {
    static int yylex(FOParser::semantic_type * yylval, OpenGraphtheory::Logic::FOLexer &scanner);
}

%start S
%token END 0 "end of file"
%token FORALL
%token EXISTS
%token DOT
%token COMMA
%token AND
%token OR
%token NOT
%token EQ
%token NEQ
%token _TRUE
%token _FALSE
%token OPEN
%token CLOSE
%token <sval> STRING

%% // Produktionen

S:
         FOFORMULA END                        { *result = $<fval>1; }
       | AFOFORMULA END                       { *result = $<fval>1; }
       ;
FOFORMULA:
       | ANDS                                 { $<fval>$ = $<fval>1; }
       | ORS                                  { $<fval>$ = $<fval>1; }

       | FORALL STRING DOT FOFORMULA          { $<fval>$ = new FO_Forall($<sval>2, $<fval>4); }
       | EXISTS STRING DOT FOFORMULA          { $<fval>$ = new FO_Exists($<sval>2, $<fval>4); }
       | FORALL STRING DOT AFOFORMULA         { $<fval>$ = new FO_Forall($<sval>2, $<fval>4); }
       | EXISTS STRING DOT AFOFORMULA         { $<fval>$ = new FO_Exists($<sval>2, $<fval>4); }
       ;

ANDS:
         ANDS AND AFOFORMULA                  { $<fval>$ = new FO_And($<fval>1, $<fval>3); }
       | AFOFORMULA AND AFOFORMULA            { $<fval>$ = new FO_And($<fval>1, $<fval>3); }
       ;
ORS:
         ORS OR AFOFORMULA                    { $<fval>$ = new FO_Or($<fval>1, $<fval>3); }
       | AFOFORMULA OR AFOFORMULA             { $<fval>$ = new FO_Or($<fval>1, $<fval>3); }
       ;
AFOFORMULA:
         _TRUE                                { $<fval>$ = new FO_True(); }
       | _FALSE                               { $<fval>$ = new FO_False(); }
       | RELATION                             { $<fval>$ = $<fval>1; }
       | NOT RELATION                         { $<fval>$ = new FO_Not( $<fval>2 ); }
       | STRING EQ STRING                     { $<fval>$ = new FO_Eq($<sval>1, $<sval>3); }
       | STRING NEQ STRING                    { $<fval>$ = new FO_Neq($<sval>1, $<sval>3); }
       | OPEN FOFORMULA CLOSE                 { $<fval>$ = $<fval>2; }
       | OPEN AFOFORMULA CLOSE                { $<fval>$ = $<fval>2; }
       | NOT OPEN FOFORMULA CLOSE             { $<fval>$ = new FO_Not( $<fval>3 ); }
       | NOT OPEN AFOFORMULA CLOSE            { $<fval>$ = new FO_Not( $<fval>3 ); }
       ;

RELATION : STRING OPEN PARAMS                 { $<fval>$ = new FO_Atomic($<sval>1, $<pval>3); }
         ;
PARAMS: STRING CLOSE                          { $<pval>$ = new FO_Parameter($<sval>1, NULL); }
       | STRING COMMA PARAMS                  { $<pval>$ = new FO_Parameter($<sval>1, $<pval>3); }
       ;
%%

#include "../../../Headers/logic/FirstOrderLogic/fo_lexer.h"

void FOParser::error(const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
}

static int yylex(FOParser::semantic_type * yylval, OpenGraphtheory::Logic::FOLexer &scanner)
{
    return scanner.yylex(yylval);
}
