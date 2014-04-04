%code requires {

    // Many thanks to
    // http://www.thebinaryidiot.com/archives/2010/03/06/flex-bison-cpp-example/

    #include "../../../Headers/import/tikz/tikz.h"

    namespace OpenGraphtheory { namespace Import { class TIKZLexer; } }
    using namespace OpenGraphtheory::Import;
}

%skeleton "lalr1.cc"
%defines
%define namespace "OpenGraphtheory::Import"
%define parser_class_name "TIKZParser"
%parse-param { OpenGraphtheory::Import::TIKZLexer &scanner }
%parse-param { OpenGraphtheory::Import::TikzCode** result }
%lex-param { OpenGraphtheory::Import::TIKZLexer &scanner }
%union
{
    std::string* sval;
    float* fval;
    TikzCode* tval;
};

%code {
    static int yylex(TIKZParser::semantic_type * yylval, OpenGraphtheory::Import::TIKZLexer &scanner);
}

%start S
%token END 0 "end of file"
%token BEGINTIKZPICTURE
%token ENDTIKZPICTURE
%token NODE
%token DRAWNODE
%token DRAWEDGE
%token DRAWARC
%token PENCOLOR
%token BRUSHCOLOR

%token OPEN
%token CLOSE
%token OPENBRACKET
%token CLOSEBRACKET
%token OPENBRACES
%token CLOSEBRACES

%token COMMA
%token SEMICOLON
%token AT
%token TO
%token CM
%token CIRCLE
%token BCIRCLE

%token <fval> FLOAT
%token <sval> STRING

%% // Produktionen

S:
         BEGINTIKZPICTURE TIKZDOCUMENT ENDTIKZPICTURE { *result = $<tval>2; }
       ;
TIKZDOCUMENT:                                 { $<tval>$ = NULL; }
       | TIKZDOCUMENT TIKZLINE                { $<tval>$ = $<tval>2; $<tval>$->PrevLines = $<tval>1; }
       ;

TIKZLINE:
         NODE OPEN STRING CLOSE AT OPEN FLOAT CM COMMA FLOAT CM CLOSE OPENBRACES STRING CLOSEBRACES SEMICOLON
                                              { $<tval>$ = new TikzDeclareNode($<sval>3, $<fval>7, $<fval>10, $<sval>14); }
       | NODE OPEN STRING CLOSE AT OPEN FLOAT CM COMMA FLOAT CM CLOSE OPENBRACES CLOSEBRACES SEMICOLON
                                              { $<tval>$ = new TikzDeclareNode($<sval>3, $<fval>7, $<fval>10, NULL); }
       | DRAWNODE OPEN STRING CLOSE BCIRCLE FLOAT CM CLOSEBRACKET SEMICOLON
                                              { $<tval>$ = new TikzDrawNode($<sval>3, $<fval>7); }
       | DRAWNODE OPEN STRING CLOSE CIRCLE FLOAT CM CLOSE SEMICOLON
                                              { $<tval>$ = new TikzDrawNode($<sval>3, $<fval>7); }
       | DRAWEDGE FLOAT CM CLOSEBRACKET OPEN STRING CLOSE TO OPEN STRING CLOSE SEMICOLON
                                              { $<tval>$ = new TikzDrawEdge($<sval>6, $<sval>10, $<fval>2); }
       | DRAWARC FLOAT CM CLOSEBRACKET OPEN STRING CLOSE TO OPEN STRING CLOSE SEMICOLON
                                              { $<tval>$ = new TikzDrawArc($<sval>6, $<sval>10, $<fval>2); }
       | PENCOLOR OPENBRACES FLOAT COMMA FLOAT COMMA FLOAT CLOSEBRACES
                                              { $<tval>$ = new TikzDefinePencolor($<fval>3, $<fval>5, $<fval>7); }
       | BRUSHCOLOR OPENBRACES FLOAT COMMA FLOAT COMMA FLOAT CLOSEBRACES
                                              { $<tval>$ = new TikzDefineBrushcolor($<fval>3, $<fval>5, $<fval>7); }
       ;
%%

#include "../../../Headers/import/tikz/tikz_lexer.h"

void TIKZParser::error(const TIKZParser::location_type &loc, const std::string &msg)
{
    std::cerr << "Error: " << msg << std::endl;
}

static int yylex(TIKZParser::semantic_type * yylval, OpenGraphtheory::Import::TIKZLexer &scanner)
{
    return scanner.yylex(yylval);
}
