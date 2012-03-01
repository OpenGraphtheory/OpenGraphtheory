
#ifndef __SIMPLE_XML_CPP
	#define __SIMPLE_XML_CPP

	#include "simple_xml.h"


    XML_Element::~XML_Element()
    {

    }

    XML_Comment::~XML_Comment()
    {

    }

    XML_Text::~XML_Text()
    {

    }


    XML::XML(XML* Parent)
    {
        parent = Parent;
    }

	XML::~XML()
	{
		for(list<XML_Element*>::iterator it = children.begin(); it != children.end(); it++)
			delete(*it);
	}

	void XML::WriteToStream(ostream& os, int level) const
	{
		os << string(level,'\t');
		os << '<' << name;
		for(list<pair<string,string> >::const_iterator it = attributes.begin(); it != attributes.end(); it++)
			os << ' ' << it->first << "=\"" << it->second << '\"';
		if(children.size() <= 0)
		{
			os << " />\n";
		}
		else
		{
			os << ">\n";
			for(list<XML_Element*>::const_iterator it = children.begin(); it != children.end(); it++)
				(*it)->WriteToStream(os, level+1);
			os << string(level,'\t') << "</" << name << ">\n";
		}
	}

	list<XML*> XML::FindChildren(string named) const
	{
		list<XML*> result;
		for(list<XML_Element*>::const_iterator i = children.begin(); i != children.end(); i++)
		{
			XML* c = dynamic_cast<XML*>(*i);
			if(c != NULL) // if instance of class XML_Element
			{
				if(c->name == named)
				{
					result.push_back(c);
				}
			}
		}
		return result;
	}

	string XML::GetAttribute(string propname, string def) const
	{
		for(list<pair<string,string> >::const_iterator i = attributes.begin(); i != attributes.end(); i++)
			if(i->first == propname)
				return i->second;
		return def;
	}

	int XML::GetAttributeAsInt(string propname, int def) const
	{
			stringstream s, t;
			s << def;
			t << GetAttribute(propname, s.str());
			int result;
			t >> result;
			return result;
	}

	float XML::GetAttributeAsFloat(string propname, float def) const
	{
			stringstream s, t;
			s << def;
			t << GetAttribute(propname, s.str());
			float result;
			t >> result;
			return result;
	}

    string XML::InnerText(bool TrimStrings) const
    {
        string result;
        for(list<XML_Element*>::const_iterator i = children.begin(); i != children.end(); i++)
            result += (*i)->InnerText(TrimStrings);
        return result;
    }




	void XML_Comment::WriteToStream(ostream& os, int level) const
	{
		if(text.size() <= 0)
			return;
		if(text.size() == 1)
		{
			os << string(level,'\t') << "<!-- " << text[0] << "-->\n";
			return;
		}
		os << string(level,'\t') << "<!--\n";
		for(vector<string>::const_iterator it = text.begin(); it != text.end(); it++)
			os << string(level,'\t') << "    " << (*it) << "\n";
		os << string(level,'\t') << "-->\n";
	}

    string XML_Comment::InnerText(bool TrimStrings) const
    {
        return "";
    }



	void XML_Text::WriteToStream(ostream& os, int level) const
	{
		for(list<string>::const_iterator it = text.begin(); it != text.end(); it++)
			os << string(level,'\t') << (*it) << '\n';
	}

    string XML_Text::InnerText(bool TrimStrings) const
    {
        string result;
        for(list<string>::const_iterator i = text.begin(); i != text.end(); i++)
        {
            if(TrimStrings)
            {
                string temp = *i;

                while(temp.length() > 0 && (temp[0] == ' ' || temp[0] == '\n'))
                    temp.erase(0,1);

                if(temp.length() > 0)
                    while(temp[temp.length()-1] == ' ' || temp[temp.length()-1] == '\n')
                        temp.erase(temp.length()-1);

                result += temp;
            }
            else
                result += *i;
        }
        return result;
    }



	ostream& operator << (ostream& os, const XML& xml)
	{
		for(list<XML_Element*>::const_iterator it = xml.children.begin(); it != xml.children.end(); it++)
			(*it)->WriteToStream(os,0);
		return os;
	}


	inline char skip_until(istream &is, string expected_end, string error_chars, char eof_result);
	inline string read_until(istream& is, string expected_end, string error_chars);
	inline char skip_while(istream &is, string skip, char eof_result);
	inline string uppercase(string);
	int LevenshteinDistance(string,string);

	istream& operator >> (istream& is, XML& xml)
	{
		XML* current = &xml;
		XML* temp;
		char nextchar;
		string str, ustr;
		bool empty = false;
		unsigned int MinLevenshteinDistance,TempLevenshteinDistance; XML* MinLevenshteinNode;

		scanning_for_tag:
			empty = false;
			nextchar = skip_while(is," \t\n", '<');
			if(nextchar != '<')
				goto reading_text;

		inside_tag:
			nextchar = skip_while(is," \t\n<", '/');
			if(nextchar == '/')
			{
				is.get();
				goto closing_tag;
			}
			else if(nextchar == '!')
			{
				is.get();
				goto reading_comment;
			}
			else if(nextchar == '?') // XML header
			{
				skip_until(is, ">", "", '>');
				goto scanning_for_tag;
			}

			current->children.push_back(new XML);
			static_cast<XML*>(current->children.back())->parent = current;
			current = static_cast<XML*>(current->children.back());
			current->name = read_until(is, "", " \t\n>/=\"");

		reading_attributes:
			nextchar = skip_while(is," \t\n",'>');
			if(nextchar == '>')
			{
				is.get();
				if(empty)
				{
					current = current->parent;
					if(current->parent == NULL)
						return is;
				}

				goto scanning_for_tag;
			}

			if(nextchar == '/')
			{
				is.get();
				empty=true;
				goto reading_attributes;
			}

			current->attributes.push_back(pair<string,string>("",""));
			current->attributes.back().first = read_until(is, " \t\n", "=/\"><");
			skip_until(is, "=", "/\"><", '=');
			nextchar = skip_until(is, "\"\'", "/><", '\"');
			current->attributes.back().second = read_until(is, string(1,nextchar), "><");
			goto reading_attributes;

		reading_text:

			current->children.push_back(new XML_Text);
			do
			{
				str = read_until(is, "\n", "<>");
				static_cast<XML_Text*>(current->children.back())->text.push_back(str);
				nextchar = skip_while(is, "\t ", '<');

			} while(nextchar != '<');

			goto inside_tag;

		reading_comment:

			if(is.peek() == '-') //if(is.peek() != '-') {cerr << "Simple XML Parser: found <! that doesn\'t start a comment\n"; goto inside_tag;}
                is.get();
			if(is.peek() == '-') // if(is.peek() != '-') {cerr << "Simple XML Parser: found <!- that doesn\'t start a comment\n"; goto inside_tag;}
                is.get();

			current->children.push_back(new XML_Comment);

			do
			{
				skip_while(is, "\t ", '>');
				str = "";
				while(true)
				{
					str += read_until(is, "", "\n-<>");
					if(is.peek() == '\n')
						break;
 					else if(is.peek() == -1)
 					{
						cerr << "Simple XML Parser: unexpected end of input-stream\n";
						static_cast<XML_Comment*>(current->children.back())->text.push_back(str);
						goto scanning_for_tag;
					}
					else if(is.peek() == '>')
					{
						//cerr << "Simple XML Parser: comments must be ended by --> and not by a single >\n";
						break;
					}
					else if(is.peek() == '<')
					{
						cerr << "Simple XML Parser: misplaced < inside a comment\n";
						break;
					}

					is.get(); // take away the -
					if(is.peek() != '-')
					{
						str += "-";
						continue;
					}

					is.get(); // take away the -
					while(is.peek() == '-') // more than 2 '-': append them to str
						str += string(1,is.get());

					if(is.peek() == '>')
						break;
					str += "--";
				}

				static_cast<XML_Comment*>(current->children.back())->text.push_back(str);
				nextchar = is.peek();

			} while(nextchar != '>');

			is.get();
			goto scanning_for_tag;

		closing_tag:
			str = read_until(is, "", "> \t\n");
			skip_until(is, ">", "<", '>');
			if(str == current->name)
			{
				current = current->parent;
			}
			else
			{

				// maybe the tag closes a higher level - eg <a><b></a>
				ustr = uppercase(str);
				for(temp = current; temp->parent != NULL; temp = temp->parent)
					if(temp->name == str)
						goto set_current_to_temp_parent;

				// maybe the tag has a wrong case eg <a><b><c></B></a>
				for(temp = current; temp->parent != NULL; temp = temp->parent)
					if(uppercase(temp->name) == ustr)
						goto set_current_to_temp_parent;

				// maybe the tag is misspelled eg <a><foo><c></BOO></a>
				MinLevenshteinDistance = LevenshteinDistance(ustr, uppercase(current->name));
				MinLevenshteinNode = current;
				for(temp = current; temp->parent != NULL; temp = temp->parent)
				{
					TempLevenshteinDistance = LevenshteinDistance(ustr, uppercase(temp->name));
					if(TempLevenshteinDistance < MinLevenshteinDistance)
					{
						MinLevenshteinDistance = TempLevenshteinDistance;
						MinLevenshteinNode = temp;
					}
				}

				if(MinLevenshteinDistance <= max(str.size(),MinLevenshteinNode->name.size())/3)
				{
					temp = MinLevenshteinNode;
					goto set_current_to_temp_parent;
				} // if Levenshtein-Distance > 33% of word-length, don't suspect it to belong together

				// give up
				cerr << "Simple XML Parser: lonely closing tag </" << str << ">\n";
				if(current->parent != NULL)
					goto scanning_for_tag;

				set_current_to_temp_parent:
				for(; current != temp; current = current->parent)
					cerr << "Simple XML Parser: tag <" << current->name << "> not closed by a </" << current->name << "> tag\n";
				if(str != temp->name && ustr == uppercase(temp->name))
					cerr << "Simple XML Parser: tag <" << temp->name << "> closed by <" << str << ">. Well-formed XML requires the same case for them!\n";
				if(ustr != uppercase(temp->name))
					cerr << "Simple XML Parser: tag <" << temp->name << "> not closed. Suspecting </" << str << "> to be misspelled.\n";
				current = temp->parent;

			}
			if(current->parent != NULL)
				goto scanning_for_tag;

		return is;
	}


	inline char skip_until(istream &is, string expected_end, string error_chars, char eof_result)
	{
		char read;
		int exp = expected_end.size()-1;
		int err = error_chars.size()-1;

		while(true)
		{
			if(is.peek() == -1)
				return eof_result;

			read = is.get();
			for(int i = exp; i >= 0; --i)
				if(expected_end[i] == read)
					return read;

			for(int i = err; i >= 0; --i)
			{
                if(error_chars[i] == read)
				{
					is.unget();
					return read;
				}
			}
		}
	}

	inline string read_until(istream& is, string expected_end, string error_chars)
	{
		string result;
		char read;
		int exp = expected_end.size()-1;
		int err = error_chars.size()-1;

		while(true)
		{
			if(is.peek() == -1)
				return result;

			read = is.get();
			for(int i = exp; i >= 0; --i)
				if(expected_end[i] == read)
					return result;

			for(int i = err; i >= 0; --i)
			{
				if(error_chars[i] == read)
				{
					is.unget();
					return result;
				}
			}
			result += read;
		}
	}

	inline char skip_while(istream &is, string skip, char eof_result)
	{
		char read;
		int s = skip.size()-1;

		back:
			if(is.peek() == -1)
				return eof_result;

			read = is.get();
			for(int i = s; i >= 0; --i)
				if(skip[i] == read)
					goto back;

		is.unget();
		return read;
	}

	inline string uppercase(string str)
	{
		for(int i = str.size()-1; i >= 0; --i)
			if(str[i] >= 'a' && str[i] <='z')
				str[i] += 'A'-'a';
		return str;
	}

	int LevenshteinDistance(string s, string t)
	{
		vector<vector<int> > d(s.size(), vector<int>(t.size(),0));
		for(unsigned int i = 0; i < s.size(); i++)
			d[i][0] = i;
		for(unsigned int j = 0; j < t.size(); j++)
			d[0][j] = j;
		for(unsigned int i = 1; i < s.size(); i++)
			for(unsigned int j = 1; j < t.size(); j++)
				d[i][j] = min(d[i-1][j  ] + 1,    // insertion
				          min(d[i]  [j-1] + 1,    // deletion
				              d[i-1][j-1] + (s[i] != t[j]))); // substitution
		return d[s.size()-1][t.size()-1];
	}


#endif
