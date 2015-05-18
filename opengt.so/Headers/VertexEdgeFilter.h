
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
                virtual bool VertexAllowed(Vertex* v) = 0;
        };

        class EdgeFilter
        {
            public:
                virtual ~EdgeFilter();
                virtual bool EdgeAllowed(Edge* e) = 0;
        };


        class ForbiddenVertexFilter : public VertexFilter
        {
            private:
                Vertex* ForbiddenVertex;
            public:
                ForbiddenVertexFilter(Vertex* ForbiddenVertex);
                bool VertexAllowed(Vertex* v);
        };

        class ForbiddenVerticesFilter : public VertexFilter
        {
            private:
                VertexSet ForbiddenVertices;
            public:
                ForbiddenVerticesFilter();
                ForbiddenVerticesFilter(VertexSet& ForbiddenVertices);
                void ForbidVertex(Vertex* ForbiddenVertex);
                void AllowVertex(Vertex* AllowedVertex);

                bool VertexAllowed(Vertex* v);
        };

        class AllowedVerticesFilter : public VertexFilter
        {
            private:
                VertexSet AllowedVertices;
            public:
                AllowedVerticesFilter();
                AllowedVerticesFilter(VertexSet& AllowedVertices);
                void ForbidVertex(Vertex* ForbiddenVertex);
                void AllowVertex(Vertex* AllowedVertex);

                bool VertexAllowed(Vertex* v);
        };

        class ForbiddenEdgeFilter : public EdgeFilter
        {
            private:
                Edge* ForbiddenEdge;
            public:
                ForbiddenEdgeFilter(Edge* ForbiddenEdge);
                bool EdgeAllowed(Edge* e);
        };

        class ForbiddenEdgesFilter : public EdgeFilter
        {
            private:
                EdgeSet ForbiddenEdges;
            public:
                ForbiddenEdgesFilter();
                ForbiddenEdgesFilter(EdgeSet& ForbiddenEdges);
                void ForbidEdge(Edge* ForbiddenEdge);
                void AllowEdge(Edge* AllowedEdge);

                bool EdgeAllowed(Edge* e);
        };

        class AllowedEdgesFilter : public EdgeFilter
        {
            private:
                EdgeSet AllowedEdges;
            public:
                AllowedEdgesFilter();
                AllowedEdgesFilter(EdgeSet& AllowedEdges);
                void ForbidEdge(Edge* ForbiddenEdge);
                void AllowEdge(Edge* AllowedEdge);

                bool EdgeAllowed(Edge* e);
        };
    }

#endif
