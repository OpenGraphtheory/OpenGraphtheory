
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




    ForbiddenVertexFilter::ForbiddenVertexFilter(Graph::VertexIterator ForbiddenVertex)
    {
        this->ForbiddenVertex = ForbiddenVertex;
    }

	bool ForbiddenVertexFilter::VertexAllowed(Graph::VertexIterator v)
	{
		return v != ForbiddenVertex;
	}




    ForbiddenVerticesFilter::ForbiddenVerticesFilter()
    {
    }

    ForbiddenVerticesFilter::ForbiddenVerticesFilter(set<Graph::VertexIterator> ForbiddenVertices)
    {
        this->ForbiddenVertices = ForbiddenVertices;
    }

    void ForbiddenVerticesFilter::ForbidVertex(Graph::VertexIterator ForbiddenVertex)
    {
        ForbiddenVertices.insert(ForbiddenVertex);
    }

    void ForbiddenVerticesFilter::AllowVertex(Graph::VertexIterator Vertex)
    {
        ForbiddenVertices.erase(Vertex);
    }

	bool ForbiddenVerticesFilter::VertexAllowed(Graph::VertexIterator v)
	{
		return ForbiddenVertices.find(v) == ForbiddenVertices.end();
	}




    ForbiddenEdgeFilter::ForbiddenEdgeFilter(Graph::EdgeIterator ForbiddenEdge)
    {
        this->ForbiddenEdge = ForbiddenEdge;
    }

	bool ForbiddenEdgeFilter::EdgeAllowed(Graph::EdgeIterator e)
	{
		return e != ForbiddenEdge;
	}




    ForbiddenEdgesFilter::ForbiddenEdgesFilter()
    {
    }

    ForbiddenEdgesFilter::ForbiddenEdgesFilter(set<Graph::EdgeIterator> ForbiddenEdges)
    {
        this->ForbiddenEdges = ForbiddenEdges;
    }

    void ForbiddenEdgesFilter::ForbidEdge(Graph::EdgeIterator ForbiddenEdge)
    {
        ForbiddenEdges.insert(ForbiddenEdge);
    }

    void ForbiddenEdgesFilter::AllowEdge(Graph::EdgeIterator Edge)
    {
        ForbiddenEdges.erase(Edge);
    }

	bool ForbiddenEdgesFilter::EdgeAllowed(Graph::EdgeIterator e)
	{
		return ForbiddenEdges.find(e) == ForbiddenEdges.end();
	}




}
