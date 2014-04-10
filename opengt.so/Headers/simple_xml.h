
#ifndef __OPENGRAPHTHEORY_SIMPLE_XML_H
	#define __OPENGRAPHTHEORY_SIMPLE_XML_H

	#include<iostream>
	#include<sstream>
	#include<vector>
	#include<list>
    #include<string>

    namespace OpenGraphtheory
    {
        namespace XML
        {


            class XML_Element  // just a common base-class
            {
                public:
                    virtual void WriteToStream(std::ostream& os, int level = 0) const = 0;
                    virtual ~XML_Element();
                    virtual std::string InnerText(bool TrimStrings = true) const = 0;

                    static void WriteXmlString(std::ostream& os, std::string str);
            };

            class XML : public XML_Element
            {
                public:
                    std::string name;
                    std::list<std::pair<std::string,std::string> > attributes;
                    std::list<XML_Element*> children;
                    XML* parent;

                    XML(XML* Parent=NULL);
                    XML(std::string name, XML* Parent=NULL);
                    ~XML();
                    void WriteToStream(std::ostream&, int level = 0) const;
                    std::list<XML*> FindChildren(std::string named) const;
                    std::string GetAttribute(std::string propname, std::string def="") const;
                    int GetAttributeAsInt(std::string propname, int def=0) const;
                    float GetAttributeAsFloat(std::string propname, float def=0) const;
                    std::string InnerText(bool TrimStrings = true) const;

                    void AddChild(XML_Element*);
                    void AddAttribute(std::string name, std::string value);
            };

            class XML_Comment : public XML_Element
            {
                public:
                    XML_Comment();
                    XML_Comment(std::string text);

                    std::vector<std::string> text;
                    void WriteToStream(std::ostream& os, int level= 0) const;
                    ~XML_Comment();
                    std::string InnerText(bool TrimStrings = true) const;
            };

            class XML_Text : public XML_Element
            {
                public:
                    XML_Text();
                    XML_Text(std::string text);
                    std::list<std::string> text;
                    void WriteToStream(std::ostream&, int level = 0) const;
                    ~XML_Text();
                    std::string InnerText(bool TrimStrings = true) const;
            };

            extern std::ostream& operator<<(std::ostream& os, const XML& xml);
            extern std::istream& operator>>(std::istream& is, XML& xml);
            std::string XmlStringToInternalString(std::string);
            std::string uppercase(std::string);
        }
    }

#endif
