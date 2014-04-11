%{
#include "../../../Headers/import/gml/gml_lexer.h"
#include "../../../Headers/import/gml/gml.h"

// #define yyterminate() return TIKZParser::token::END

%}

%option c++
%option case-insensitive
%option yyclass="OpenGraphtheory::Import::GMLLexer"
%option full
%option noyywrap
%option nounput
%option batch
%option yylineno
%option prefix="Import_GML_"

%%

[\ \n\t\r]+                                   {}

"["                                           {
                                                  GMLTreeNode* frame = new GMLTreeNode();
                                                  GMLStack.top()->Children.push_back(std::pair<std::string, GMLValue*>(key, new GMLValueNode(frame)));
                                                  GMLStack.push(frame);
                                                  key="";
                                              }

"]"                                           {
                                                  if(GMLStack.size() > 1)
                                                  {
                                                      GMLStack.pop();
                                                      key = "";
                                                  }
                                                  else
                                                      std::cerr << "GML document contains too many \']\' tokens" << std::endl;
                                              }

[a-zA-Z][0-9a-zA-Z]*                          {
                                                  key = std::string(yytext);
                                              }

\"[^\"]*\"                                    {
                                                  GMLStack.top()->Children.push_back(std::pair<std::string,GMLValue*>(key, new GMLValueString(std::string(yytext).substr(1,yyleng-2))));
                                                  key = "";
                                              }

[\+\-]?[0-9]+                                 {
                                                  GMLStack.top()->Children.push_back(std::pair<std::string,GMLValue*>(key, new GMLValueInt(atoi(yytext))));
                                                  key = "";
                                              }

[\+\-]?([0-9]*\.)?[0-9]+                      {
                                                  GMLStack.top()->Children.push_back(std::pair<std::string,GMLValue*>(key, new GMLValueFloat(atof(yytext))));
                                                  key = "";
                                              }

.                                             {
                                                  std::cerr << "lexical error (" << yylineno << "): unexpected character " << yytext << std::endl;
                                              }

%%

OpenGraphtheory::Import::GMLLexer::GMLLexer(std::istream& is) : Import_GML_FlexLexer(&is)
{
}

int OpenGraphtheory::Import::GMLLexer::yylex(GMLTreeNode* GMLTreeRoot)
{
    GMLStack.push(GMLTreeRoot);
    int result = yylex();
    while(!GMLStack.empty())
        GMLStack.pop();
    return result;
}

