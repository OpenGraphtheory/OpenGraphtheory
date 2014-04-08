
#include "../../Headers/export/stringtranslator.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Export
    {

        StringTranslator::StringTranslator()
        {
        }

        StringTranslator::~StringTranslator()
        {
        }

        string StringTranslator::Translate(string String)
        {
            string result;
            for(string::iterator i = String.begin(); i != String.end(); i++)
            {
                if((int)*i == -61)
                {
                    i++;
                    switch((int)*i)
                    {
                        // ü
                        case -68: result += SpecialCharacter_uuml(); break;
                        // ö
                        case -74: result += SpecialCharacter_ouml(); break;
                        // ä
                        case -92: result += SpecialCharacter_auml(); break;
                        // Ä
                        case -124: result += SpecialCharacter_Auml(); break;
                        // Ö
                        case -106: result += SpecialCharacter_Ouml(); break;
                        // Ü
                        case -100: result += SpecialCharacter_Uuml(); break;
                        // ß
                        case -97: result += SpecialCharacter_szlig(); break;
                    }
                    continue;
                }

                if(SpecialCharacters.find(*i) != SpecialCharacters.end())
                    result += SpecialCharacters[*i];
                else
                    result += *i;
            }
            return result;
        }

        string StringTranslator::SpecialCharacter_auml()
        {
            return "ä";
        }

        string StringTranslator::SpecialCharacter_Auml()
        {
            return "Ä";
        }

        string StringTranslator::SpecialCharacter_uuml()
        {
            return "ü";
        }

        string StringTranslator::SpecialCharacter_Uuml()
        {
            return "Ü";
        }

        string StringTranslator::SpecialCharacter_ouml()
        {
            return "ö";
        }

        string StringTranslator::SpecialCharacter_Ouml()
        {
            return "Ö";
        }

        string StringTranslator::SpecialCharacter_szlig()
        {
            return "ß";
        }




        StringTranslatorLatex::StringTranslatorLatex()
            : StringTranslator()
        {
            SpecialCharacters['{'] = "\\{";
            SpecialCharacters['}'] = "\\}";
            SpecialCharacters['&'] = "\\&";
            SpecialCharacters['%'] = "\\%";
            SpecialCharacters['$'] = "\\$";
            SpecialCharacters['#'] = "\\#";
            SpecialCharacters['_'] = "\\_";
            SpecialCharacters['\\'] = "{\\textbackslash}";
            SpecialCharacters['^'] = "{\\textasciicircum}";
            SpecialCharacters['~'] = "{\\textasciitilde}";
        }

        StringTranslatorLatex::~StringTranslatorLatex()
        {
        }

        string StringTranslatorLatex::SpecialCharacter_auml()
        {
            return "\\\"a";
        }
        string StringTranslatorLatex::SpecialCharacter_Auml()
        {
            return "\\\"A";
        }
        string StringTranslatorLatex::SpecialCharacter_uuml()
        {
            return "\\\"u";
        }
        string StringTranslatorLatex::SpecialCharacter_Uuml()
        {
            return "\\\"U";
        }
        string StringTranslatorLatex::SpecialCharacter_ouml()
        {
            return "\\\"o";
        }
        string StringTranslatorLatex::SpecialCharacter_Ouml()
        {
            return "\\\"O";
        }
        string StringTranslatorLatex::SpecialCharacter_szlig()
        {
            return "{\\ss}";
        }



        StringTranslatorXML::StringTranslatorXML()
            : StringTranslator()
        {
            SpecialCharacters['<'] = "&lt;";
            SpecialCharacters['>'] = "&gt;";
            SpecialCharacters['&'] = "&amp;";
            SpecialCharacters['\"'] = "&quot;";
        }

        StringTranslatorXML::~StringTranslatorXML()
        {
        }

    }
}
