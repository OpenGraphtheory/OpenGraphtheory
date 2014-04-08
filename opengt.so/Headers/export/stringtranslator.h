
#ifndef __OPENGRAPHTHEORY_EXPORT_STRINGTRANSLATOR_H
    #define __OPENGRAPHTHEORY_EXPORT_STRINGTRANSLATOR_H

    #include <map>
    #include <string>

    namespace OpenGraphtheory
    {
        namespace Export
        {

            class StringTranslator
            {
                protected:

                    std::map<char, std::string> SpecialCharacters;

                    virtual std::string SpecialCharacter_auml();
                    virtual std::string SpecialCharacter_Auml();
                    virtual std::string SpecialCharacter_ouml();
                    virtual std::string SpecialCharacter_Ouml();
                    virtual std::string SpecialCharacter_uuml();
                    virtual std::string SpecialCharacter_Uuml();
                    virtual std::string SpecialCharacter_szlig();

                public:

                    StringTranslator();
                    virtual ~StringTranslator();
                    virtual std::string Translate(std::string String);
            };

            class StringTranslatorLatex : public StringTranslator
            {
                protected:

                    std::string SpecialCharacter_auml();
                    std::string SpecialCharacter_Auml();
                    std::string SpecialCharacter_ouml();
                    std::string SpecialCharacter_Ouml();
                    std::string SpecialCharacter_uuml();
                    std::string SpecialCharacter_Uuml();
                    std::string SpecialCharacter_szlig();

                public:

                    StringTranslatorLatex();
                    ~StringTranslatorLatex();
            };

            class StringTranslatorXML : public StringTranslator
            {
                public:
                    StringTranslatorXML();
                    ~StringTranslatorXML();
            };

        }
    }

#endif
