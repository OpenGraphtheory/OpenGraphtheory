
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_VERTEXSEPARATOR_VERTEXSEPARATOR_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_VERTEXSEPARATOR_VERTEXSEPARATOR_H

    #include "../../opengt.h"
    #include "../algorithms.h"
    #include <set>
    #include "../../set_helper.h"
    #include "../vertexdisjointpaths/vertexdisjointpaths.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {

            class AlgorithmVERTEXSEPARATOR : public AlgorithmVERTEXDISJOINTPATHS
            {
                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmVertexSeparatorRegistrator;
                public:
                    bool FindMinimumVertexSeparator(Graph &G, VertexSet& Sources, VertexSet& Drains, VertexSet& Separator,
                                                    VertexFilter* vertexfilter = NULL, EdgeFilter* edgefilter = NULL);
                    bool FindMinimumVertexSeparator(Graph &G, Vertex* Source, Vertex* Drain, VertexSet& Separator,
                                                    VertexFilter* vertexfilter = NULL, EdgeFilter* edgefilter = NULL);

                    void AddMinimumVertexSeparator(Graph &G, VertexSet& Sources, VertexSet& Drains, std::string SeparatorName);
                    void AddMinimumVertexSeparator(Graph &G, Vertex* Source, Vertex* Drain, std::string SeparatorName);
                    void Run(Graph& G, std::vector<std::string> parameters);
            };

        }
    }

#endif

