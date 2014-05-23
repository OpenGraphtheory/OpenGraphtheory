
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




}
