
#include "../Headers/VertexEdgeFilter.h"
using namespace std;

namespace OpenGraphtheory
{

    VertexFilter::~VertexFilter()
    {
    }

    EdgeFilter::~EdgeFilter()
    {
    }




    ForbiddenVertexFilter::ForbiddenVertexFilter(Vertex* ForbiddenVertex)
    {
        this->ForbiddenVertex = ForbiddenVertex;
    }

    bool ForbiddenVertexFilter::VertexAllowed(Vertex* v)
    {
        return v != ForbiddenVertex;
    }




    ForbiddenVerticesFilter::ForbiddenVerticesFilter()
    {
    }

    ForbiddenVerticesFilter::ForbiddenVerticesFilter(VertexSet& ForbiddenVertices)
    {
        this->ForbiddenVertices = ForbiddenVertices;
    }

    void ForbiddenVerticesFilter::ForbidVertex(Vertex* ForbiddenVertex)
    {
        ForbiddenVertices.insert(ForbiddenVertex);
    }

    void ForbiddenVerticesFilter::AllowVertex(Vertex* Vertex)
    {
        ForbiddenVertices.erase(Vertex);
    }

    bool ForbiddenVerticesFilter::VertexAllowed(Vertex* v)
    {
        return !ForbiddenVertices.contains(v);
    }





    AllowedVerticesFilter::AllowedVerticesFilter()
    {
    }

    AllowedVerticesFilter::AllowedVerticesFilter(VertexSet& AllowedVertices)
    {
        this->AllowedVertices = AllowedVertices;
    }

    void AllowedVerticesFilter::ForbidVertex(Vertex* ForbiddenVertex)
    {
        AllowedVertices.erase(ForbiddenVertex);
    }

    void AllowedVerticesFilter::AllowVertex(Vertex* Vertex)
    {
        AllowedVertices.insert(Vertex);
    }

    bool AllowedVerticesFilter::VertexAllowed(Vertex* v)
    {
        return AllowedVertices.contains(v);
    }





    ForbiddenEdgeFilter::ForbiddenEdgeFilter(Edge* ForbiddenEdge)
    {
        this->ForbiddenEdge = ForbiddenEdge;
    }

    bool ForbiddenEdgeFilter::EdgeAllowed(Edge* e)
    {
        return e != ForbiddenEdge;
    }




    ForbiddenEdgesFilter::ForbiddenEdgesFilter()
    {
    }

    ForbiddenEdgesFilter::ForbiddenEdgesFilter(EdgeSet& ForbiddenEdges)
    {
        this->ForbiddenEdges = ForbiddenEdges;
    }

    void ForbiddenEdgesFilter::ForbidEdge(Edge* ForbiddenEdge)
    {
        ForbiddenEdges.insert(ForbiddenEdge);
    }

    void ForbiddenEdgesFilter::AllowEdge(Edge* Edge)
    {
        ForbiddenEdges.erase(Edge);
    }

    bool ForbiddenEdgesFilter::EdgeAllowed(Edge* e)
    {
        return !ForbiddenEdges.contains(e);
    }





    AllowedEdgesFilter::AllowedEdgesFilter()
    {
    }

    AllowedEdgesFilter::AllowedEdgesFilter(EdgeSet& AllowedEdges)
    {
        this->AllowedEdges = AllowedEdges;
    }

    void AllowedEdgesFilter::ForbidEdge(Edge* ForbiddenEdge)
    {
        AllowedEdges.erase(ForbiddenEdge);
    }

    void AllowedEdgesFilter::AllowEdge(Edge* AllowedEdge)
    {
        AllowedEdges.insert(AllowedEdge);
    }

    bool AllowedEdgesFilter::EdgeAllowed(Edge* e)
    {
        return AllowedEdges.contains(e);
    }

}
