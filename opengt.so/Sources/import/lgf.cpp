
#include "../../Headers/import/lgf.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Import
    {

        FactoryRegistrator<ImportFilter> ImportFilterLGF::ImportFilterLgfRegistrator(
            &ImportFilter::ImportFilterFactory, "lgf", new DefaultInstantiator<ImportFilter, ImportFilterLGF>(
                "lgf", "LEMON Graph Format","http://lemon.cs.elte.hu/pub/doc/latest/a00002.html"));


        string readtoken(istream& is)
        {
            string token = "";

            // 0 = only whitespace so far
            // 1 = reading token
            // 2 = reading quoted token
            int state = 0;
            char c = is.peek(); // update eof info
            while(!is.eof())
            {
                switch(c)
                {
                    case ' ':
                    case '\t':
                        if(state == 0) // ignore leading whitespace
                            break;
                        if(state == 1) // whitespace behind unquoted token => token finished
                            return token;
                        if(state == 2) // whitespace inside quoted token
                            token += c;
                        break;

                    case '\n':
                        if(state == 0)
                        {
                            is.get();
                            return "\n";
                        }
                        else if(state == 1) // whitespace behind unquoted token => token finished
                            return token;   // (\n is not fetched - it becomes the next token)
                        else if(state == 2)
                            token += c;     // quoted token may contain linebreaks
                        break;

                    case '\"':
                        if(state == 0) // start of quoted token
                            state = 2;
                        else if(state == 2) // quote behind quoted token => token finished
                        {
                            is.get();
                            return token;
                        }
                        break;

                    default:
                        token += c;
                        if(state != 2)
                            state = 1;
                        break;
                }

                is.get();
                c = is.peek();
            }

            return token;
        }

        Graph ImportFilterLGF::Import(istream& is)
        {
            list<LGFSection*> LGFSections;
            LGFSection* currentsection = NULL;
            list<string> *currentrow = NULL;

            // -1 = comment line
            // 0 = beginning of line
            // 1 = inside normal line
            // 2 = ignoring \n after new section
            // 3 = reading column map
            int state = 0;
            int currentcolumnnumber = 0;
            is.peek(); // update eof info
            while(!is.eof())
            {
                string token = readtoken(is);

                switch(state)
                {
                    case -1:
                        if(token == "\n")
                            state = 0;
                        break;

                    case 0:
                        if(token == "\n") // ignore empty lines
                            break;

                        if(token[0]=='@') // new section
                        {
                            currentsection = new LGFSection;
                            LGFSections.push_back(currentsection);
                            currentsection->name = token;
                            state = 2;
                            currentcolumnnumber = 0;
                        }
                        else if(token[0]=='#') // comment
                        {
                            state = -1;
                        }
                        else
                        {
                            currentsection->rows.push_back( list<string>() );
                            currentrow = &(currentsection->rows.back());
                            currentrow->push_back(token);
                            state = 1;
                        }
                        break;

                    case 1:
                        if(token == "\n")
                            state = 0;
                        else
                            currentrow->push_back(token);
                        break;

                    case 2:
                        //if(token == "\n")
                            state = 3;
                        break;

                    case 3:
                        if(token == "\n")
                            state = 0;
                        else
                        {
                            currentsection->columnnames.push_back(token);
                            currentsection->column_number[token] = currentcolumnnumber;
                            currentcolumnnumber++;
                        }
                        break;
                }

                is.peek(); // update eof info
            }


            Graph result;
            map<string, Vertex*> VertexLGFIDs;

            // read nodes
            for(list<LGFSection*>::iterator section = LGFSections.begin(); section != LGFSections.end(); section++)
                if((*section)->name == "@nodes")
                {
                    int labelcolumn = (*section)->column_number.find("label") != (*section)->column_number.end() ? (*section)->column_number["label"] : -1;
                    if(labelcolumn < 0) // column "label" is required by the LGF standard
                        continue;

                    int titlecolumn = (*section)->column_number.find("title") != (*section)->column_number.end() ? (*section)->column_number["title"] : -1;
                    int sizecolumn = (*section)->column_number.find("size") != (*section)->column_number.end() ? (*section)->column_number["size"] : -1;
                    int coordinatescolumn = (*section)->column_number.find("coordinates") != (*section)->column_number.end() ? (*section)->column_number["coordinates"] : -1;

                    for(list<list<string> >::iterator row = (*section)->rows.begin(); row != (*section)->rows.end(); row++)
                    {
                        VertexIterator v = result.AddVertex();

                        list<string>::iterator label = row->begin(); // label
                        std::advance(label, labelcolumn);
                        VertexLGFIDs[*label] = *v;

                        if(titlecolumn >= 0) // title
                        {
                            list<string>::iterator title = row->begin();
                            std::advance(title, titlecolumn);
                            (*v)->SetLabel(*title);
                        }

                        if(sizecolumn >= 0) // size
                        {
                            list<string>::iterator vsize = row->begin();
                            std::advance(vsize, sizecolumn);
                            stringstream s;
                            s << *vsize;
                            float f;
                            s >> f;
                            (*v)->SetWeight(f);
                        }

                        if(coordinatescolumn >= 0) // coordinates
                        {
                            list<string>::iterator coordinates = row->begin();
                            std::advance(coordinates, coordinatescolumn);
                            stringstream s;
                            for(string::iterator i = coordinates->begin(); i != coordinates->end(); i++)
                                s << ( (( '0' <= *i && *i <= '9' ) || (*i=='.')) ? *i : ' ' );

                            Coordinates c;
                            float f;
                            s >> f;
                            c.push_back(f);
                            s >> f;
                            c.push_back(f);
                            (*v)->SetCoordinates(c);
                        }

                    }

                }

            // read edges
            for(list<LGFSection*>::iterator section = LGFSections.begin(); section != LGFSections.end(); section++)
                if((*section)->name == "@arcs" || (*section)->name =="@edges")
                {
                    int titlecolumn = (*section)->column_number.find("title") != (*section)->column_number.end() ? (*section)->column_number["title"]+2 : -1;
                    int capacitycolumn = (*section)->column_number.find("capacity") != (*section)->column_number.end() ? (*section)->column_number["capacity"]+2 : -1;

                    for(list<list<string> >::iterator row = (*section)->rows.begin(); row != (*section)->rows.end(); row++)
                    {
                        Vertex *v1 = NULL;
                        Vertex *v2 = NULL;
                        list<string>::iterator r = row->begin();
                        if(r != row->end())
                        {
                            v1 = VertexLGFIDs.find(*r) != VertexLGFIDs.end() ? VertexLGFIDs[*r] : NULL;
                            r++;
                            if(r != row->end())
                                v2 = VertexLGFIDs.find(*r) != VertexLGFIDs.end() ? VertexLGFIDs[*r] : NULL;
                        }
                        if(v1 == NULL || v2 == NULL)
                            continue;

                        EdgeIterator e;
                        if((*section)->name == "@arcs")
                            e = result.AddArc(v1, v2);
                        else
                            e = result.AddEdge(v1, v2);

                        if(titlecolumn >= 0) // title
                        {
                            list<string>::iterator title = row->begin();
                            std::advance(title, titlecolumn);
                            (*e)->SetLabel(*title);
                        }

                        if(capacitycolumn >= 0) // title
                        {
                            list<string>::iterator capacity = row->begin();
                            std::advance(capacity, capacitycolumn);
                            stringstream s;
                            s << *capacity;
                            float f;
                            s >> f;
                            (*e)->SetWeight(f);
                        }
                    }
                }

            return result;
        }

    }
}
