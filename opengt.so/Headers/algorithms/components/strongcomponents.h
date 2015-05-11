
#ifndef __OPENGRAPHTHEORY_ALGORITHMS_COMPONENTS_STRONGCOMPONENTS_H
    #define __OPENGRAPHTHEORY_ALGORITHMS_COMPONENTS_STRONGCOMPONENTS_H

    #include "../algorithms.h"
    #include "../path/path.h"
    #include "../../VertexEdgeFilter.h"
    #include<string>
    #include<map>
    #include<vector>

    namespace OpenGraphtheory
    {
        namespace Algorithms
        {
            class AlgorithmSTRONGCOMPONENTS : public Algorithm
            {
                public:
                    void Run(Graph &G, std::vector<std::string> parameters);
                    void AddStrongComponents(Graph &G, std::string compnentsname);
                    void FindStrongComponents(Graph &G, std::map<Vertex*, int>& ComponentOfVertex, std::vector<std::vector<Vertex*> >& VerticesInComponent,
                                              VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL);
                protected:
                    static MultiFactoryRegistrator<Algorithm> AlgorithmStrongComponentsRegistrator;
                    void FindComponents(Graph &G, std::map<Vertex*, int>& ComponentOfVertex, std::vector<std::vector<Vertex*> >& VerticesInComponent,
                                        VertexFilter *vertexfilter=NULL, EdgeFilter *edgefilter=NULL, bool StrongComponents=true);
            };
        }
    }

#endif

