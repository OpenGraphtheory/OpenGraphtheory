
#include "../../../Headers/import/dot/dot.h"
#include "../../../Headers/import/dot/dot_lexer.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Import;

#include "dot_parser.hpp"


namespace OpenGraphtheory
{
	namespace Import
	{

		FactoryRegistrator<ImportFilter> ImportFilterDOT::ImportFilterDOTRegistrator(
			&ImportFilter::ImportFilterFactory, "dot", new DefaultInstantiator<ImportFilter, ImportFilterDOT>(
				"dot", "Fileformat of the Graphviz project", "http://www.graphviz.org/doc/info/lang.html"));


        Graph ImportFilterDOT::Import(std::istream& is)
        {
			OpenGraphtheory::Import::DOTLexer l(is);
			DotCode* dotdocument;
			DOTParser p(l, &dotdocument);
			if(p.parse() == 0)
			{
                Graph result;
                DotContext context(&result);
                dotdocument->Load(&context);
                delete dotdocument;
                return result;
			}
			else
			{
                throw "Error parsing dot structure";
			}
        }

		// --------------------------------------------------------------------------------------------------------

        DotContext::DotContext(Graph* G)
        {
            this->G = G;
        }

        DotContext::~DotContext()
        {
        }

		// --------------------------------------------------------------------------------------------------------

        DotCode::DotCode()
        {
        }

        DotCode::~DotCode()
        {
            if(PrevLines != NULL)
                delete PrevLines;
        }

		void DotCode::Load(DotContext* context)
		{
            if(PrevLines != NULL)
                PrevLines->Load(context);
            DoLoad(context);
		}

		// --------------------------------------------------------------------------------------------------------

		DotNode::DotNode(string* name, DotParameter* parameters)
		{
            this->name = name;
            this->parameters = parameters;
		}

		DotNode::~DotNode()
		{
            delete name;
            delete parameters;
		}

        void DotNode::DoLoad(DotContext* context)
        {
            Graph::VertexIterator v = context->G->AddVertex();
            context->nodes[*name] = v;
            if(parameters != NULL)
                parameters->LoadVertex(&v);
        }


		// --------------------------------------------------------------------------------------------------------

		DotEdge::DotEdge(std::string* from, std::string* to, DotParameter* parameters)
		{
            this->from = from;
            this->to = to;
            this->parameters = parameters;
		}

		DotEdge::~DotEdge()
		{
            delete from;
            delete to;
            delete parameters;
		}

        void DotEdge::DoLoad(DotContext* context)
        {
            Graph::VertexIterator vfrom = context->nodes[*from];
            Graph::VertexIterator vto = context->nodes[*to];

            Graph::EdgeIterator e;
            if(parameters == NULL || !(parameters->ContainsDirectedAttribute()))
            {
                e = context->G->AddEdge(vfrom, vto);
            }
            else
            {
                e = context->G->AddArc(vfrom, vto);
            }

            if(parameters != NULL)
                parameters->LoadEdge(&e);
        }

		// --------------------------------------------------------------------------------------------------------

        DotParameter::DotParameter(string* name, string* value)
        {
            this->name = name;
            this->value = value;
        }

        DotParameter::~DotParameter()
        {
            delete name;
            delete value;
        }

        void DotParameter::LoadVertex(Graph::VertexIterator* v)
        {
            if(PrevParameters != NULL)
                PrevParameters->LoadVertex(v);

            if(*name == "label")
            {
                v->SetLabel(*value);
            }
            else if(*name == "width")
            {
                float r;
                stringstream s;
                s << *value;
                s >> r;
                v->SetWeight(r * 2.54); // dot uses inches, gxl uses cm
            }
            else if(*name == "pos")
            {
                size_t prevcomma = 0;
                size_t nextcomma;
                vector<float> coordinates;
                do
                {
                    nextcomma = value->find(',', prevcomma);
                    string scoordinate;
                    if(nextcomma >= 0)
                        scoordinate = value->substr(prevcomma,nextcomma);
                    else
                        scoordinate = value->substr(prevcomma);
                    prevcomma = nextcomma+1;

                    float coordinate;
                    stringstream s;
                    s << scoordinate;
                    s >> coordinate;

                    coordinates.push_back(coordinate);
                } while(nextcomma >= 0);
                v->SetCoordinates(coordinates);
            }
            else // arbitrary attribute
            {
                v->Attributes().Unset(*name);
                v->Attributes().Add(*name, "string");

                Attribute* pAttr = v->Attributes().GetAttribute(*name);
                StringAttribute* sAttr = dynamic_cast<StringAttribute*>(pAttr);
                if(sAttr != NULL)
                    sAttr->Value = *value;
            }
        }

        void DotParameter::LoadEdge(Graph::EdgeIterator* e)
        {
            if(PrevParameters != NULL)
                PrevParameters->LoadEdge(e);

            if(*name == "label")
            {
                e->SetLabel(*value);
            }
            else if(*name == "dir")
            {
                // nothing (this case is handled by ContainsDirectedAttribute)
            }
            else if(*name == "penwidth")
            {
                float w;
                stringstream s;
                s << *value;
                s >> w;
                e->SetWeight(w * 2.54); // dot uses inches, open graphtheory uses cm
            }
            else // arbitrary attribute
            {
                e->Attributes().Unset(*name);
                e->Attributes().Add(*name, "string");

                Attribute* pAttr = e->Attributes().GetAttribute(*name);
                StringAttribute* sAttr = dynamic_cast<StringAttribute*>(pAttr);
                if(sAttr != NULL)
                    sAttr->Value = *value;
            }
        }

        bool DotParameter::ContainsDirectedAttribute()
        {
            if(*name == "dir")
            {
                if(*value == "forward")
                    return true;
                else
                    return false;
            }
            if(PrevParameters != NULL)
                return PrevParameters->ContainsDirectedAttribute();
            return false;
        }

	} // namespace Logic
} // namespace OpenGraphtheory

