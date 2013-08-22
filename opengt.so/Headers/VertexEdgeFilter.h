
#ifndef __OPENGRAPHTHEORY_VERTEXEDGEFILTER_H
	#define __OPENGRAPHTHEORY_VERTEXEDGEFILTER_H

	#include "opengt.h"
	#include<set>

	namespace OpenGraphtheory
	{

		class VertexFilter
		{
			public:
				virtual ~VertexFilter();
				virtual bool VertexAllowed(Graph::VertexIterator v) = 0;
		};

		class EdgeFilter
		{
			public:
				virtual ~EdgeFilter();
				virtual bool EdgeAllowed(Graph::EdgeIterator e) = 0;
		};


        class ForbiddenVertexFilter : public VertexFilter
        {
            private:
                Graph::VertexIterator ForbiddenVertex;
            public:
                ForbiddenVertexFilter(Graph::VertexIterator ForbiddenVertex);
                bool VertexAllowed(Graph::VertexIterator v);
        };

        class ForbiddenVerticesFilter : public VertexFilter
        {
            private:
                std::set<Graph::VertexIterator> ForbiddenVertices;
            public:
                ForbiddenVerticesFilter();
                ForbiddenVerticesFilter(std::set<Graph::VertexIterator> ForbiddenVertices);
                void ForbidVertex(Graph::VertexIterator ForbiddenVertex);
                void AllowVertex(Graph::VertexIterator Vertex);

                bool VertexAllowed(Graph::VertexIterator v);
        };

        class ForbiddenEdgeFilter : public EdgeFilter
        {
            private:
                Graph::EdgeIterator ForbiddenEdge;
            public:
                ForbiddenEdgeFilter(Graph::EdgeIterator ForbiddenEdge);
                bool EdgeAllowed(Graph::EdgeIterator e);
        };

        class ForbiddenEdgesFilter : public EdgeFilter
        {
            private:
                std::set<Graph::EdgeIterator> ForbiddenEdges;
            public:
                ForbiddenEdgesFilter();
                ForbiddenEdgesFilter(std::set<Graph::EdgeIterator> ForbiddenEdges);
                void ForbidEdge(Graph::EdgeIterator ForbiddenEdge);
                void AllowEdge(Graph::EdgeIterator Edge);

                bool EdgeAllowed(Graph::EdgeIterator e);
        };

	}

#endif
