
#include "../../../Headers/import/tikz/tikz.h"
#include "../../../Headers/import/tikz/tikz_lexer.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Import;

#include "tikz_parser.hpp"


namespace OpenGraphtheory
{
	namespace Import
	{

		FactoryRegistrator<ImportFilter> ImportFilterTIKZ::ImportFilterTIKZRegistrator(
			&ImportFilter::ImportFilterFactory, "tikz", new DefaultInstantiator<ImportFilter, ImportFilterTIKZ>(
				"tikz", "TikZ graphics for LaTeX", "http://en.wikipedia.org/wiki/PGF/TikZ"));


        Graph ImportFilterTIKZ::Import(std::istream& is)
        {
			OpenGraphtheory::Import::TIKZLexer l(is);
			TikzCode* tikzdocument;
			TIKZParser p(l, &tikzdocument);
			if(p.parse() == 0)
			{
                Graph result;
                TikzContext context(&result);
                tikzdocument->Load(&context);
                delete tikzdocument;
                return result;
			}
			else
			{
                throw "Error parsing tikz structure";
			}
        }

		// --------------------------------------------------------------------------------------------------------

        TikzContext::TikzContext(Graph* G)
        {
            this->G = G;
        }

        TikzContext::~TikzContext()
        {
        }

		// --------------------------------------------------------------------------------------------------------

        TikzCode::TikzCode()
        {
            this->PrevLines = NULL;
        }

        TikzCode::~TikzCode()
        {
            if(PrevLines != NULL)
                delete PrevLines;
        }

		void TikzCode::Load(TikzContext* context)
		{
            if(PrevLines != NULL)
                PrevLines->Load(context);
            DoLoad(context);
		}

		// --------------------------------------------------------------------------------------------------------

		TikzDeclareNode::TikzDeclareNode(string* name, float* xCm, float* yCm, string* label)
		{
            this->name = name;
            this->xCm = xCm;
            this->yCm = yCm;
            this->label = label;
		}

		TikzDeclareNode::~TikzDeclareNode()
		{
            delete name;
            delete xCm;
            delete yCm;
            delete label;
		}

        void TikzDeclareNode::DoLoad(TikzContext* context)
        {
            Graph::VertexIterator v = context->G->AddVertex(*xCm, *yCm, label != NULL ? *label : "");
            context->nodes[*name] = v;
        }

		// --------------------------------------------------------------------------------------------------------

		TikzDrawEdge::TikzDrawEdge(std::string* from, std::string* to, float* linewidth)
		{
            this->from = from;
            this->to = to;
            this->linewidth = linewidth;
		}

		TikzDrawEdge::~TikzDrawEdge()
		{
            delete from;
            delete to;
            delete linewidth;
		}

        void TikzDrawEdge::DoLoad(TikzContext* context)
        {
            Graph::VertexIterator vfrom = context->nodes[*from];
            Graph::VertexIterator vto = context->nodes[*to];
            context->G->AddEdge(vfrom, vto, "", *linewidth);
        }
		// --------------------------------------------------------------------------------------------------------

		TikzDrawArc::TikzDrawArc(std::string* from, std::string* to, float* linewidth)
            : TikzDrawEdge(from, to, linewidth)
		{
		}

		TikzDrawArc::~TikzDrawArc()
		{
		}

        void TikzDrawArc::DoLoad(TikzContext* context)
        {
            Graph::VertexIterator vfrom = context->nodes[*from];
            Graph::VertexIterator vto = context->nodes[*to];
            context->G->AddArc(vfrom, vto, "", *linewidth);
        }

		// --------------------------------------------------------------------------------------------------------

		TikzDrawNode::TikzDrawNode(std::string* name, float* radius)
		{
            this->name = name;
            this->radius = radius;
		}

		TikzDrawNode::~TikzDrawNode()
		{
            delete name;
            delete radius;
		}

        void TikzDrawNode::DoLoad(TikzContext* context)
        {
            Graph::VertexIterator v = context->nodes[*name];
            v.SetWeight(*radius);
        }

		// --------------------------------------------------------------------------------------------------------

		TikzDefinePencolor::TikzDefinePencolor(float* r, float* g, float* b)
		{
            this->r = r;
            this->g = g;
            this->b = b;
		}

		TikzDefinePencolor::~TikzDefinePencolor()
		{
            delete r;
            delete g;
            delete b;
		}

        void TikzDefinePencolor::DoLoad(TikzContext* context)
        {
            context->PenColor.Red = *r;
            context->PenColor.Green = *g;
            context->PenColor.Blue = *b;
        }

		// --------------------------------------------------------------------------------------------------------

		TikzDefineBrushcolor::TikzDefineBrushcolor(float* r, float* g, float* b)
            : TikzDefinePencolor(r, g, b)
		{
		}

		TikzDefineBrushcolor::~TikzDefineBrushcolor()
		{
		}

        void TikzDefineBrushcolor::DoLoad(TikzContext* context)
        {
            context->BrushColor.Red = *r;
            context->BrushColor.Green = *g;
            context->BrushColor.Blue = *b;
        }


	} // namespace Logic
} // namespace OpenGraphtheory

