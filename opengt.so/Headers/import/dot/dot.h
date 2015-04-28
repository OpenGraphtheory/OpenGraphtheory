
#ifndef __OPENGRAPHTHEORY_IMPORT_DOT_DOT_H
    #define __OPENGRAPHTHEORY_IMPORT_DOT_DOT_H

    #include<string>
    #include<map>
    #include "../import.h"
    #include "../../visualize/color.h"

    namespace OpenGraphtheory
    {
        namespace Import
        {
            class ImportFilterDOT : public ImportFilter
            {
                protected:
                    static FactoryRegistrator<ImportFilter> ImportFilterDOTRegistrator;
                public:
                    Graph Import(std::istream& is);
            };

            class DotContext
            {
                public:
                    Graph* G;
                    std::map<std::string, Vertex*> nodes;

                    DotContext(Graph* G);
                    ~DotContext();
            };

            class DotCode
            {
                public:
                    DotCode* PrevLines;

                    DotCode();
                    virtual ~DotCode();
                    void Load (DotContext* context);
                    virtual void DoLoad(DotContext* context) = 0;
            };

            class DotParameter
            {
                public:
                    std::string* name;
                    std::string* value;
                    DotParameter* PrevParameters;

                    DotParameter(std::string* name, std::string* value);
                    ~DotParameter();
                    void LoadVertex(Vertex* v);
                    void LoadEdge(Edge* e);
                    bool ContainsDirectedAttribute();
            };

            class DotNode : public DotCode
            {
                protected:
                    std::string* name;
                    DotParameter* parameters;
                public:
                    DotNode(std::string* name, DotParameter* parameters);
                    ~DotNode();
                    void DoLoad(DotContext* context);
            };

            class DotEdge : public DotCode
            {
                protected:
                    std::string* from;
                    std::string* to;
                    DotParameter* parameters;
                public:
                    DotEdge(std::string* from, std::string* to, DotParameter* parameters);
                    ~DotEdge();
                    void DoLoad(DotContext* context);
            };

        } // namespace Import
    } // namespace OpenGraphtheory

#endif
