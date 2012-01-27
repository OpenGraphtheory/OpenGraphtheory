
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
			virtual ~XML_Element() {}
	};

	class XML : public XML_Element
	{
		public:
			string name;
			list<pair<string,string> > attributes;
			list<XML_Element*> children;
			XML* parent;

			XML(XML* Parent=NULL) {parent=Parent;}
			~XML();
			void WriteToStream(ostream&, int level) const;
			list<XML*> FindChildren(string named) const;
			string GetAttribute(string propname, string def="") const;
			int GetAttributeAsInt(string propname, int def=0) const;
			float GetAttributeAsFloat(string propname, float def=0) const;
	};

	class XML_Comment : public XML_Element
	{
		public:
			vector<string> text;
			void WriteToStream(ostream& os, int level) const;
			~XML_Comment(){}
	};

	class XML_Text : public XML_Element
	{
		public:
			list<string> text;
			void WriteToStream(ostream&, int level) const;
			~XML_Text(){}
	};

	extern ostream& operator<<(ostream& os, XML& xml);
	extern istream& operator>>(istream& is, XML& xml);

#endif
