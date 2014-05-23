
#ifndef __OPENGRAPHTHEORY_IMPORT_TIKZ_TIKZ_H
    #define __OPENGRAPHTHEORY_IMPORT_TIKZ_TIKZ_H

    #include<string>
	#include<map>
    #include "../import.h"
    #include "../../visualize/color.h"

    namespace OpenGraphtheory
    {
        namespace Import
        {
            class ImportFilterTIKZ : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterTIKZRegistrator;
                public:
                    Graph Import(std::istream& is);
            };


            class TikzContext
            {
                public:
                    TikzContext(Graph* G);
                    ~TikzContext();

                    Graph* G;
                    std::map<std::string, VertexIterator> nodes;
                    OpenGraphtheory::Visualization::Color PenColor;
                    OpenGraphtheory::Visualization::Color BrushColor;
            };

            class TikzCode
            {
                public:
                    TikzCode* PrevLines;

                    TikzCode();
                    virtual ~TikzCode();
                    void Load (TikzContext* context);
                    virtual void DoLoad(TikzContext* context) = 0;
            };


            class TikzDeclareNode : public TikzCode
            {
                protected:
                    std::string* name;
                    float* xCm;
                    float* yCm;
                    float* diameter;
                    std::string* label;
                public:
                    TikzDeclareNode(std::string* name, float* xCm, float* yCm, float* diameter, std::string* label);
                    ~TikzDeclareNode();
                    void DoLoad(TikzContext* context);
            };

            class TikzDrawEdge : public TikzCode
            {
                protected:
                    std::string* from;
                    std::string* to;
                    float* linewidth;
                public:
                    TikzDrawEdge(std::string* from, std::string* to, float* linewidth);
                    ~TikzDrawEdge();
                    void DoLoad(TikzContext* context);
            };

            class TikzDrawArc : public TikzDrawEdge
            {
                public:
                    TikzDrawArc(std::string* from, std::string* to, float* linewidth);
                    ~TikzDrawArc();
                    void DoLoad(TikzContext* context);
            };

            class TikzDefinePencolor : public TikzCode
            {
                protected:
                    float* r;
                    float* g;
                    float* b;
                public:
                    TikzDefinePencolor(float* r, float* g, float* b);
                    ~TikzDefinePencolor();
                    void DoLoad(TikzContext* context);
            };

            class TikzDefineBrushcolor : public TikzDefinePencolor
            {
                public:
                    TikzDefineBrushcolor(float* r, float* g, float* b);
                    ~TikzDefineBrushcolor();
                    void DoLoad(TikzContext* context);
            };

        } // namespace Import
    } // namespace OpenGraphtheory

#endif
