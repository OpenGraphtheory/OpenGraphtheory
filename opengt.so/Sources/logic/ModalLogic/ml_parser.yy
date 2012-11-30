%code requires {

    // Many thanks to
    // http://www.thebinaryidiot.com/archives/2010/03/06/flex-bison-cpp-example/

    #include<string>
    #include "../../../Headers/logic/ModalLogic/ml.h"

    namespace OpenGraphtheory { namespace Logic { class MLLexer; } }
    using namespace OpenGraphtheory::Logic;
}

%skeleton "lalr1.cc"
%defines
%define namespace "OpenGraphtheory::Logic"
%define parser_class_name "MLParser"
%parse-param { OpenGraphtheory::Logic::MLLexer &scanner }
%parse-param { OpenGraphtheory::Logic::MLFormula** result }
%lex-param { OpenGraphtheory::Logic::MLLexer &scanner }
%union
{
    std::string* sval;
    MLFormula* fval;
};

%code {
    static int yylex(MLParser::semantic_type * yylval, OpenGraphtheory::Logic::MLLexer &scanner);
}

%start S
%token END 0 "end of file"
%token DIAMOND
%token BOX
%token AND
%token OR
%token NOT
%token _TRUE
%token _FALSE
%token OPEN
%token CLOSE
%token <sval> STRING

%% // Produktionen

S:
         MLFORMULA END                        { *result = $<fval>1; }
       | AMLFORMULA END                       { *result = $<fval>1; }
       ;
MLFORMULA:
       | ANDS                                 { $<fval>$ = $<fval>1; }
       | ORS                                  { $<fval>$ = $<fval>1; }

       | DIAMOND MLFORMULA                    { $<fval>$ = new ML_Diamond($<fval>2); }
       | BOX MLFORMULA                        { $<fval>$ = new ML_Box($<fval>2); }
       | DIAMOND AMLFORMULA                   { $<fval>$ = new ML_Diamond($<fval>2); }
       | BOX AMLFORMULA                       { $<fval>$ = new ML_Box($<fval>2); }
       ;

ANDS:
         ANDS AND AMLFORMULA                  { $<fval>$ = new ML_And($<fval>1, $<fval>3); }
       | AMLFORMULA AND AMLFORMULA            { $<fval>$ = new ML_And($<fval>1, $<fval>3); }
       ;
ORS:
         ORS OR AMLFORMULA                    { $<fval>$ = new ML_Or($<fval>1, $<fval>3); }
       | AMLFORMULA OR AMLFORMULA             { $<fval>$ = new ML_Or($<fval>1, $<fval>3); }
       ;
AMLFORMULA:
         _TRUE                                { $<fval>$ = new ML_True(); }
       | _FALSE                               { $<fval>$ = new ML_False(); }
       | STRING                               { $<fval>$ = new ML_Atomic($<sval>1); }
       | NOT STRING                           { $<fval>$ = new ML_Not( new ML_Atomic($<sval>2) ); }
       | OPEN MLFORMULA CLOSE                 { $<fval>$ = $<fval>2; }
       | OPEN AMLFORMULA CLOSE                { $<fval>$ = $<fval>2; }
       | NOT OPEN MLFORMULA CLOSE             { $<fval>$ = new ML_Not( $<fval>3 ); }
       | NOT OPEN AMLFORMULA CLOSE            { $<fval>$ = new ML_Not( $<fval>3 ); }
       ;

%%

#include "../../../Headers/logic/ModalLogic/ml_lexer.h"

void MLParser::error(const MLParser::location_type &loc, const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
}

static int yylex(MLParser::semantic_type * yylval, OpenGraphtheory::Logic::MLLexer &scanner)
{
    return scanner.yylex(yylval);
}
