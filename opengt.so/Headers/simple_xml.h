
#ifndef __SIMPLE_XML_H
	#define __SIMPLE_XML_H

	#include<iostream>
	#include<sstream>
	#include<vector>
	#include<list>
	using namespace std;



	class XML_Element  // just a common base-class
	{
		public:
			virtual void WriteToStream(ostream& os, int level) const = 0;
			virtual ~XML_Element();
			virtual string InnerText(bool TrimStrings) const = 0;

			static void WriteXmlString(ostream& os, string str);
	};

	class XML : public XML_Element
	{
		public:
			string name;
			list<pair<string,string> > attributes;
			list<XML_Element*> children;
			XML* parent;

			XML(XML* Parent=NULL);
			XML(string name, XML* Parent=NULL);
			~XML();
			void WriteToStream(ostream&, int level) const;
			list<XML*> FindChildren(string named) const;
			string GetAttribute(string propname, string def="") const;
			int GetAttributeAsInt(string propname, int def=0) const;
			float GetAttributeAsFloat(string propname, float def=0) const;
			string InnerText(bool TrimStrings) const;

			void AddChild(XML_Element*);
			void AddAttribute(string name, string value);
	};

	class XML_Comment : public XML_Element
	{
		public:
            XML_Comment();
            XML_Comment(string text);

			vector<string> text;
			void WriteToStream(ostream& os, int level) const;
			~XML_Comment();
			string InnerText(bool TrimStrings) const;
	};

	class XML_Text : public XML_Element
	{
		public:
            XML_Text();
            XML_Text(string text);
			list<string> text;
			void WriteToStream(ostream&, int level) const;
			~XML_Text();
			string InnerText(bool TrimStrings) const;
	};

	extern ostream& operator<<(ostream& os, const XML& xml);
	extern istream& operator>>(istream& is, XML& xml);

#endif