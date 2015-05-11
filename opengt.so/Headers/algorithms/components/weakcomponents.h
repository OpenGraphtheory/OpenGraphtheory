
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_COMPONENTS_WEAKCOMPONENTS_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_COMPONENTS_WEAKCOMPONENTS_H

    #include "strongcomponents.h"

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmWEAKCOMPONENTS : public AlgorithmSTRONGCOMPONENTS
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    void AddWeakComponents(Graph &G, std::string compnentsname);
                    void FindWeakComponents(Graph &G, std::map<Vertex*, int>& ComponentOfVertex, std::vector<std::vector<Vertex*> >& VerticesInComponent,
                                            VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmWeakComponentsRegistrator;
            };
        }
    }

#endif

