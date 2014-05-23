
#include "../../../Headers/import/gml/gml.h"
#include "../../../Headers/import/gml/gml_lexer.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Import;

namespace OpenGraphtheory
{
	namespace Import
	{

		FactoryRegistrator<ImportFilter> ImportFilterGML::ImportFilterGMLRegistrator(
			&ImportFilter::ImportFilterFactory, "gml", new DefaultInstantiator<ImportFilter, ImportFilterGML>(
				"gml", "Graph Modelling Language",
                "http://www.fim.uni-passau.de/en/fim/faculty/chairs/theoretische-informatik/projects.html"));


        template<class T> T* GetValue(GMLTreeNode* node, string Name)
        {
            transform(Name.begin(), Name.end(), Name.begin(), ::toupper);
            for(list<pair<string, GMLValue*> >::iterator i = node->Children.begin(); i != node->Children.end(); i++)
            {
                string I = i->first;
                transform(I.begin(), I.end(), I.begin(), ::toupper);
                if(I == Name)
                    return dynamic_cast<T*>(i->second);
            }

            return NULL;
        }

        Graph ImportFilterGML::Import(std::istream& is)
        {
			OpenGraphtheory::Import::GMLLexer l(is);
			GMLTreeNode* GMLTreeRoot = new GMLTreeNode;
			l.yylex(GMLTreeRoot);

            Graph result;
            map<int, VertexIterator> VertexRegister;
            GMLValueNode* GraphNode = GetValue<GMLValueNode>(GMLTreeRoot, "GRAPH");
            if(GraphNode == NULL)
                throw "GML Document contains no top-level element \"graph\" with children";

            bool GraphDirected = false;
            GMLValueInt* directed = GetValue<GMLValueInt>(GraphNode->value, "DIRECTED");
            if(directed != NULL)
                GraphDirected = directed->value == 1;

            // Iterate Nodes
            for(list<pair<string, GMLValue*> >::iterator n = GraphNode->value->Children.begin(); n != GraphNode->value->Children.end(); n++)
            {
                string N = n->first;
                transform(N.begin(), N.end(), N.begin(), ::toupper);
                if(N != "NODE")
                    continue;
                GMLValueNode *NValue = dynamic_cast<GMLValueNode*>(n->second);
                if(NValue == NULL)
                    continue;

                GMLValueInt* id = GetValue<GMLValueInt>(NValue->value, "ID");
                if(id == NULL)
                    continue;
                if(VertexRegister.find(id->value) != VertexRegister.end())
                    throw "Multiple nodes with the same id";
                VertexIterator vi = result.AddVertex();
                VertexRegister[id->value] = vi;
                Vertex* v = *vi;

                GMLValueString* label = GetValue<GMLValueString>(NValue->value, "LABEL");
                if(label != NULL)
                    v->SetLabel(label->value);

                GMLValueNode* graphics = GetValue<GMLValueNode>(NValue->value, "GRAPHICS");
                if(graphics != NULL)
                {
                    vector<float> coordinates;

                    GMLValueFloat* xf = GetValue<GMLValueFloat>(graphics->value, "X");
                    GMLValueInt* xi = GetValue<GMLValueInt>(graphics->value, "X");
                    if(xf != NULL) coordinates.push_back(xf->value);
                    else if(xi != NULL) coordinates.push_back(xi->value);
                    else coordinates.push_back(0);

                    GMLValueFloat* yf = GetValue<GMLValueFloat>(graphics->value, "Y");
                    GMLValueInt* yi = GetValue<GMLValueInt>(graphics->value, "Y");
                    if(yf != NULL) coordinates.push_back(yf->value);
                    else if(yi != NULL) coordinates.push_back(yi->value);
                    else coordinates.push_back(0);

                    GMLValueFloat* zf = GetValue<GMLValueFloat>(graphics->value, "Z");
                    GMLValueInt* zi = GetValue<GMLValueInt>(graphics->value, "Z");
                    if(zf != NULL) coordinates.push_back(zf->value);
                    else if(zi != NULL) coordinates.push_back(zi->value);
                    // a third coordinate is not enforced

                    v->SetCoordinates(coordinates);

                    GMLValueFloat* wf = GetValue<GMLValueFloat>(graphics->value, "W");
                    GMLValueInt* wi = GetValue<GMLValueInt>(graphics->value, "W");
                    if(wf != NULL) v->SetWeight(wf->value);
                    else if(wi != NULL) v->SetWeight(wi->value);

                }

            }

            for(list<pair<string, GMLValue*> >::iterator e = GraphNode->value->Children.begin(); e != GraphNode->value->Children.end(); e++)
            {
                string E = e->first;
                transform(E.begin(), E.end(), E.begin(), ::toupper);
                if(E != "EDGE")
                    continue;
                GMLValueNode *EValue = dynamic_cast<GMLValueNode*>(e->second);
                if(EValue == NULL)
                    continue;

                GMLValueInt* source = GetValue<GMLValueInt>(EValue->value, "SOURCE");
                GMLValueInt* target = GetValue<GMLValueInt>(EValue->value, "TARGET");
                if(source == NULL || target == NULL)
                    continue;
                if(VertexRegister.find(source->value) == VertexRegister.end()
                || VertexRegister.find(target->value) == VertexRegister.end())
                    continue;

                bool Directed = GraphDirected;
                GMLValueNode* graphics = GetValue<GMLValueNode>(EValue->value, "GRAPHICS");
                if(graphics != NULL)
                {
                    GMLValueString* type = GetValue<GMLValueString>(graphics->value, "TYPE");
                    if(type != NULL)
                        Directed = type->value == "arc";
                }

                EdgeIterator ei;
                if(Directed)
                    ei = result.AddArc(VertexRegister[source->value], VertexRegister[target->value]);
                else
                    ei = result.AddEdge(VertexRegister[source->value], VertexRegister[target->value]);

                GMLValueString* label = GetValue<GMLValueString>(EValue->value, "LABEL");
                if(label != NULL)
                    (*ei)->SetLabel(label->value);
            }

            delete GMLTreeRoot;
            return result;
        }

		// --------------------------------------------------------------------------------------------------------

        GMLValue::GMLValue()
        {
        }

        GMLValue::~GMLValue()
        {
        }

        GMLTreeNode::GMLTreeNode()
        {
        }

        GMLTreeNode::~GMLTreeNode()
        {
            for(list<pair<string, GMLValue*> >::iterator i = Children.begin(); i != Children.end(); i++)
                delete i->second;
        }

        GMLValueInt::GMLValueInt(int value)
        {
            this->value = value;
        }

        GMLValueInt::~GMLValueInt()
        {
        }

        GMLValueFloat::GMLValueFloat(float value)
        {
            this->value = value;
        }

        GMLValueFloat::~GMLValueFloat()
        {
        }

        GMLValueString::GMLValueString(std::string value)
        {
            this->value = value;
        }

        GMLValueString::~GMLValueString()
        {
        }

        GMLValueNode::GMLValueNode(GMLTreeNode* value)
        {
            this->value = value;
        }

        GMLValueNode::~GMLValueNode()
        {
            if(value != NULL)
                delete value;
        }


	} // namespace Logic
} // namespace OpenGraphtheory

