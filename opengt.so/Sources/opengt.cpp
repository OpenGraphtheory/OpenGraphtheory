
#include "../Headers/opengt.h"

using namespace std;

namespace OpenGraphtheory
{

	int Vertex::Vertex_IDs = 0;
	int Edge::Edge_IDs = 0;


	Factory<GraphObject> GraphObject::GraphObjectFactory;
    FactoryRegistrator<GraphObject> Graph::GraphRegistrator(
        &GraphObject::GraphObjectFactory, "graph", new DefaultInstantiator<GraphObject, Graph>("graph", "", ""));
    FactoryRegistrator<GraphObject> Vertex::VertexRegistrator(
        &GraphObject::GraphObjectFactory, "node", new DefaultInstantiator<GraphObject, Vertex>("node", "", ""));
    FactoryRegistrator<GraphObject> Edge::EdgeRegistrator(
        &GraphObject::GraphObjectFactory, "edge", new DefaultInstantiator<GraphObject, Edge>("edge", "", ""));
    FactoryRegistrator<GraphObject> Edge::RelRegistrator(
        &GraphObject::GraphObjectFactory, "rel", new DefaultInstantiator<GraphObject, Edge>("rel", "", ""));


	/// \defgroup constructorsdestructors ''Constructors, Destructors and low-level Methods''
	// @{

		/// \brief GraphObject Constructor (abstract base class for Graphs, Vertices and Edges)
        GraphObject::GraphObject()
        {
            attributes = NULL; // this makes sure we don't get into an undefined state, if the allocation fails
		    attributes = new AttributeCollection();
        }

		/// \brief GraphObject Destructor
        GraphObject::~GraphObject()
        {
            if(attributes != NULL)
                delete attributes;
        }


		/// \brief Vertex Constructor (protected, accessible for class "Graph")
		Vertex::Vertex(Graph* owner, int ID)
            : GraphObject()
		{
			if(ID <= 0)
				ID = ++Vertex_IDs;
			this->ID = ID;

			Owner = owner;
			Connections = new VertexEdgeConnectionSet;
		}

		/// \brief Vertex Destructor
		Vertex::~Vertex()
		{
            delete Connections;
		}


		/// \brief Edge Constructor (protected, accessible for class "Graph")
		Edge::Edge(Graph* owner, int ID)
            : GraphObject()
		{
			if(ID <= 0)
				ID = ++Edge_IDs;
			this->ID = ID;

			Owner = owner;
            Connections = new VertexEdgeConnectionSet;
		}

		/// \brief Vertex Destructor
		Edge::~Edge()
		{
            delete Connections;
		}


		/// \brief Graph Constructor, creates a graph with "size" vertices
		Graph::Graph(int size)
            : GraphObject()
		{
			Vertices = NULL;
			Edges = NULL;
			Vertex_ID_to_pointer = NULL;
			Edge_ID_to_pointer = NULL;

			Vertices = new VertexSet;
			Edges = new EdgeSet;
			Vertex_ID_to_pointer = new map<int, Vertex*>;
			Edge_ID_to_pointer = new map<int, Edge*>;

			for(int i = 0; i < size; i++)
				AddVertex();
		}

		/// \brief Graph Copy-Constructor
		Graph::Graph(const Graph& G)
            : GraphObject()
		{
			Vertices = NULL;
			Edges = NULL;
			Vertex_ID_to_pointer = NULL;
			Edge_ID_to_pointer = NULL;

			Vertices = new VertexSet;
			Edges = new EdgeSet;
			Vertex_ID_to_pointer = new map<int, Vertex*>;
			Edge_ID_to_pointer = new map<int, Edge*>;

			operator=(G);
		}

		#if __cplusplus >= 201103L
		Graph::Graph(Graph&& G)
            : GraphObject()
		{
			this->attributes = G.attributes;
			this->Vertices = G.Vertices;
			this->Edges = G.Edges;
            this->Vertex_ID_to_pointer = G.Vertex_ID_to_pointer;
            this->Edge_ID_to_pointer = G.Edge_ID_to_pointer;

			G.attributes = NULL;
			G.vertices = NULL;
			G.edges = NULL;
			G.Vertex_ID_to_pointer = NULL;
			G.Edge_ID_to_pointer = NULL;

			#error Owner der Vertices und Edges wird nicht umgesetzt
		}
		#endif

		/// \brief Graph Destructor
		Graph::~Graph()
		{
			Clear();

			if(Vertices != NULL)
                delete Vertices;
			if(Edges != NULL)
                delete Edges;
			if(Vertex_ID_to_pointer != NULL)
                delete Vertex_ID_to_pointer;
			if(Edge_ID_to_pointer != NULL)
                delete Edge_ID_to_pointer;
        }

        int Graph::NumberOfVertices() const
        {
            if(Vertices == NULL)
                return 0;
            return Vertices->size();
        }

        int Graph::NumberOfEdges() const
        {
            if(Edges == NULL)
                return 0;
            return Edges->size();
        }

        /// \brief Operator to copy a graph
        void Graph::operator=(const Graph& G)
        {
            Clear();

            *attributes = *(G.attributes);

            /// copy vertices
            for(ConstVertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                VertexIterator vnew = this->InternalAddVertex((*v)->GetID());
                *((*vnew)->attributes) = *((*v)->attributes);
            }

            /// copy edges
            for(ConstEdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                EdgeIterator ne = InternalAddEdge(NULL, NULL, true, (*e)->GetID());

                /// copy connections
                for(ConstVertexEdgeConnectionIterator conn = (*e)->BeginConnections(); conn != (*e)->EndConnections(); conn++)
                {
                    int id = (*conn)->GetVertex()->GetID();
                    Vertex* target = (*Vertex_ID_to_pointer)[id];
                    (*ne)->AddConnection(target, (*conn)->GetDirection());
                }

                *((*ne)->attributes) = *((*e)->attributes);
            }
        }

        /// \brief Remove all vertices and edges
        void Graph::Clear()
        {
            GraphObject::Clear();

            while(NumberOfEdges() > 0)
                RemoveEdge(BeginEdges());
            while(NumberOfVertices() > 0)
                RemoveVertex(BeginVertices());
        }

	// @}


    void GraphObject::Clear()
    {
        if(attributes != NULL)
            attributes->Clear();
    }

    Attribute* GraphObject::AddAttribute(string AttributeName, string AttributeType)
    {
        if(attributes == NULL)
            return NULL;
        return attributes->Add(AttributeName, AttributeType);
    }

    Attribute* GraphObject::GetAttribute(string name)
    {
        if(attributes == NULL)
            return NULL;
        return attributes->GetAttribute(name);
    }

    void GraphObject::RemoveAttribute(string name)
    {
        if(attributes != NULL)
            attributes->Unset(name);
    }

    Edge* VertexEdgeConnection::GetEdge()
    {
        return edge;
    }

    Vertex* VertexEdgeConnection::GetVertex()
    {
        return vertex;
    }

    VertexEdgeConnection::Direction VertexEdgeConnection::GetDirection() const
    {
        return direction;
    }

    size_t Vertex::NumberOfConnections() const
    {
        return Connections->size();
    }

    size_t Edge::NumberOfConnections() const
    {
        return Connections->size();
    }

	/// \defgroup basictests ''Testing basic properties''
	// @{


        /// \defgroup adjacenceincidence ''Adjacence, Incidence''
        // @{

            /// \brief Test whether the Vertex is adjacent to the given vertex
            bool Vertex::Adjacent(const Vertex* to,
                    bool UndirectedToUndirected, bool UndirectedToPositive, bool UndirectedToNegative,
                    bool PositiveToUndirected,   bool PositiveToPositive,   bool PositiveToNegative,
                    bool NegativeToUndirected,   bool NegativeToPositive,   bool NegativeToNegative) const
            {
                return GetEdge(to, UndirectedToUndirected, UndirectedToPositive, UndirectedToNegative,
                                   PositiveToUndirected,   PositiveToPositive,   PositiveToNegative,
                                   NegativeToUndirected,   NegativeToPositive,   NegativeToNegative) != NULL;
            }

            bool Vertex::UnderlyingAdjacent(const Vertex* to) const
            {
                for(ConstVertexEdgeConnectionIterator it = Connections->begin(); it != Connections->end(); it++)
                    if((*it)->GetEdge()->Incident(to,1,1,1))
                        return true;
                return false;
            }

            VertexSet Vertex::CollectNeighbors(
                    bool UndirectedToUndirected, bool UndirectedToPositive, bool UndirectedToNegative,
                    bool PositiveToUndirected,   bool PositiveToPositive,   bool PositiveToNegative,
                    bool NegativeToUndirected,   bool NegativeToPositive,   bool NegativeToNegative)
            {
                VertexSet result;

                for(VertexEdgeConnectionIterator v2e = BeginConnections(); v2e != EndConnections(); v2e++)
                {
                    VertexEdgeConnection::Direction v2edirection = (*v2e)->GetDirection();
                    if(!(
                        ((UndirectedToUndirected || UndirectedToNegative || UndirectedToPositive) && v2edirection == VertexEdgeConnection::Undirected)
                        ||((PositiveToUndirected || PositiveToNegative || PositiveToPositive) && v2edirection == VertexEdgeConnection::VertexToEdge)
                        ||((NegativeToUndirected || NegativeToNegative || NegativeToPositive) && v2edirection == VertexEdgeConnection::EdgeToVertex)
                    ))
                        continue;
                    Edge* e = (*v2e)->GetEdge();

                    for(VertexEdgeConnectionIterator e2v = e->BeginConnections(); e2v != e->EndConnections(); e2v++)
                    {
                        VertexEdgeConnection::Direction e2vdirection = (*e2v)->GetDirection();

                        bool Addable = false;
                        switch(v2edirection)
                        {
                            case  VertexEdgeConnection::Undirected:
                                switch(e2vdirection)
                                {
                                    case VertexEdgeConnection::Undirected: Addable = UndirectedToUndirected; break;
                                    case VertexEdgeConnection::VertexToEdge: Addable = UndirectedToNegative; break;
                                    case VertexEdgeConnection::EdgeToVertex: Addable = UndirectedToPositive; break;
                                    default: throw "illegal direction value";
                                }
                                break;
                            case  VertexEdgeConnection::VertexToEdge:
                                switch(e2vdirection)
                                {
                                    case VertexEdgeConnection::Undirected: Addable = PositiveToUndirected; break;
                                    case VertexEdgeConnection::VertexToEdge: Addable = PositiveToNegative; break;
                                    case VertexEdgeConnection::EdgeToVertex: Addable = PositiveToPositive; break;
                                    default: throw "illegal direction value";
                                }
                                break;
                            case  VertexEdgeConnection::EdgeToVertex:
                                switch(e2vdirection)
                                {
                                    case VertexEdgeConnection::Undirected: Addable = NegativeToUndirected; break;
                                    case VertexEdgeConnection::VertexToEdge: Addable = NegativeToNegative; break;
                                    case VertexEdgeConnection::EdgeToVertex: Addable = NegativeToPositive; break;
                                    default: throw "illegal direction value";
                                }
                                break;
                            default: throw "illegal direction value";
                        }

                        if(Addable)
                            result.insert((*e2v)->GetVertex());
                    }
                }

                return result;
            }

            EdgeSet Vertex::CollectIncidentEdges(bool Undirected, bool VertexToEdge, bool EdgeToVertex)
            {
                EdgeSet result;
                for(VertexEdgeConnectionIterator c = Connections->begin(); c != Connections->end(); c++)
                {
                    if((VertexToEdge && (*c)->GetDirection() == VertexEdgeConnection::VertexToEdge)
                       || (EdgeToVertex && (*c)->GetDirection() == VertexEdgeConnection::EdgeToVertex)
                       || (Undirected && (*c)->GetDirection() == VertexEdgeConnection::Undirected))
                        result.insert((*c)->GetEdge());
                }
                return result;
            }

            VertexSet Edge::CollectIncidentVertices(bool Undirected, bool EdgeToVertex, bool VertexToEdge)
            {
                VertexSet result;
                for(VertexEdgeConnectionIterator conn = BeginConnections(); conn != EndConnections(); conn++)
                {
                    if((Undirected && (*conn)->GetDirection() == VertexEdgeConnection::Undirected)
                    ||(EdgeToVertex && (*conn)->GetDirection() == VertexEdgeConnection::EdgeToVertex)
                    ||(VertexToEdge && (*conn)->GetDirection() == VertexEdgeConnection::VertexToEdge))
                        result.insert((*conn)->GetVertex());
                }
                return result;
            }


            VertexSet Vertex::UnderlyingNeighborhood()
            {
                return CollectNeighbors(true,true,true, true,true,true, true,true,true);
            }

            VertexSet Vertex::Successors()
            {
                return CollectNeighbors(false,false,false, false,true,false, false,false,false);
            }

            VertexSet Vertex::Predecessors()
            {
                return CollectNeighbors(false,false,false, false,false,false, false,false,true);
            }




            Edge* Vertex::GetEdge(const Vertex* to,
                    bool UndirectedToUndirected, bool UndirectedToPositive, bool UndirectedToNegative,
                    bool PositiveToUndirected,   bool PositiveToPositive,   bool PositiveToNegative,
                    bool NegativeToUndirected,   bool NegativeToPositive,   bool NegativeToNegative) const

            {
                for(ConstVertexEdgeConnectionIterator v2e = BeginConnections(); v2e != EndConnections(); v2e++)
                {
                    VertexEdgeConnection::Direction v2edirection = (*v2e)->GetDirection();
                    if(!(
                        ((UndirectedToUndirected || UndirectedToNegative || UndirectedToPositive) && v2edirection == VertexEdgeConnection::Undirected)
                        ||((PositiveToUndirected || PositiveToNegative || PositiveToPositive) && v2edirection == VertexEdgeConnection::VertexToEdge)
                        ||((NegativeToUndirected || NegativeToNegative || NegativeToPositive) && v2edirection == VertexEdgeConnection::EdgeToVertex)
                    ))
                        continue;
                    Edge* e = (*v2e)->GetEdge();

                    for(VertexEdgeConnectionIterator e2v = e->BeginConnections(); e2v != e->EndConnections(); e2v++)
                    {
                        if(v2e == e2v || (*e2v)->GetVertex() != to)
                            continue;

                        VertexEdgeConnection::Direction e2vdirection = (*e2v)->GetDirection();

                        bool IsCandidate = false;
                        switch(v2edirection)
                        {
                            case  VertexEdgeConnection::Undirected:
                                switch(e2vdirection)
                                {
                                    case VertexEdgeConnection::Undirected: IsCandidate = UndirectedToUndirected; break;
                                    case VertexEdgeConnection::VertexToEdge: IsCandidate = UndirectedToNegative; break;
                                    case VertexEdgeConnection::EdgeToVertex: IsCandidate = UndirectedToPositive; break;
                                    default: throw "illegal direction value";
                                }
                                break;
                            case  VertexEdgeConnection::VertexToEdge:
                                switch(e2vdirection)
                                {
                                    case VertexEdgeConnection::Undirected: IsCandidate = PositiveToUndirected; break;
                                    case VertexEdgeConnection::VertexToEdge: IsCandidate = PositiveToNegative; break;
                                    case VertexEdgeConnection::EdgeToVertex: IsCandidate = PositiveToPositive; break;
                                    default: throw "illegal direction value";
                                }
                                break;
                            case  VertexEdgeConnection::EdgeToVertex:
                                switch(e2vdirection)
                                {
                                    case VertexEdgeConnection::Undirected: IsCandidate = NegativeToUndirected; break;
                                    case VertexEdgeConnection::VertexToEdge: IsCandidate = NegativeToNegative; break;
                                    case VertexEdgeConnection::EdgeToVertex: IsCandidate = NegativeToPositive; break;
                                    default: throw "illegal direction value";
                                }
                                break;
                            default: throw "illegal direction value";
                        }

                        if(IsCandidate)
                            return e;
                    }
                }

                return NULL;
            }



            /// \brief Test whether the Edge is incident to the given vertex
            bool Edge::Incident(const Vertex* to, bool Undirected, bool Positive, bool Negative) const
            {
                for(ConstVertexEdgeConnectionIterator conn = BeginConnections(); conn != EndConnections(); conn++)
                {
                    VertexEdgeConnection::Direction dir = (*conn)->GetDirection();
                    if((  (dir == VertexEdgeConnection::Undirected && Undirected)
                        ||(dir == VertexEdgeConnection::EdgeToVertex && Undirected)
                        ||(dir == VertexEdgeConnection::VertexToEdge && Undirected)
                        ) && to == (*conn)->GetVertex())
                        return true;
                }
                return false;
            }

            bool Edge::UnderlyingIncident(const Vertex* to) const
            {
                return Incident(to, 1, 1, 1);
            }

        // @}



        /// \defgroup directedundirected ''Directed, Undirected, Mixed''
        // @{

            /// \brief Test whether the edge is undirected (has no directed connections)
            bool Edge::IsUndirected()
            {
                for(VertexEdgeConnectionIterator conn = BeginConnections(); conn != EndConnections(); conn++)
                    if((*conn)->GetDirection() != VertexEdgeConnection::Undirected)
                        return false;
                return true;
            }

            /// \brief Test whether the graph has no directed edges
            bool Graph::IsUndirected()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if(!(*e)->IsUndirected())
                        return false;
                return true;
            }

            /// \brief Test whether the edge is directed (has no undirected connections)
            bool Edge::IsDirected()
            {
                return !IsUndirected();
            }

            /// \brief Test whether the graph has no undirected edges
            bool Graph::IsDirected()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if(!(*e)->IsDirected())
                        return false;
                return true;
            }

            /// \brief Test whether the graph has directed and undirected edges
            bool Graph::IsMixed()
            {
                return !IsDirected() && !IsUndirected();
            }

        // @}


        /// \defgroup loopmultigraph ''Loops, Multigraphs''
        // @{

            /// \brief Test whether the Edge is an undirected loop
            bool Edge::IsUndirectedLoop()
            {
                if(NumberOfConnections() == 2)
                {
                    VertexEdgeConnectionIterator v1 = BeginConnections();
                    VertexEdgeConnectionIterator v2 = v1+1;
                    if((*v1)->GetDirection() == VertexEdgeConnection::Undirected
                        && (*v2)->GetDirection() == VertexEdgeConnection::Undirected
                        && (*v1)->GetVertex() == (*v2)->GetVertex())
                        return true;
                }
                return false;
            }

            /// \brief Test whether the graph contains an undirected loop
            bool Graph::HasUndirectedLoops()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if((*e)->IsUndirectedLoop())
                        return true;
                return false;
            }

            /// \brief Test whether the Edge is a directed loop
            bool Edge::IsDirectedLoop()
            {
                if(NumberOfConnections() == 2)
                {
                    VertexEdgeConnectionIterator v1 = BeginConnections();
                    VertexEdgeConnectionIterator v2 = v1+1;
                    if((*v1)->GetDirection() == VertexEdgeConnection::VertexToEdge
                        && (*v2)->GetDirection() == VertexEdgeConnection::EdgeToVertex
                        && (*v1)->GetVertex() == (*v2)->GetVertex())
                        return true;
                    if((*v1)->GetDirection() == VertexEdgeConnection::EdgeToVertex
                        && (*v2)->GetDirection() == VertexEdgeConnection::VertexToEdge
                        && (*v1)->GetVertex() == (*v2)->GetVertex())
                        return true;
                }
                return false;
            }

            /// \brief Test whether the graph contains a directed loop
            bool Graph::HasDirectedLoops()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if((*e)->IsDirectedLoop())
                        return true;
                return false;
            }

            /// \brief Test whether the Edge is a loop
            bool Edge::IsLoop()
            {
                return IsUndirectedLoop() || IsDirectedLoop();
            }

            /// \brief Test whether the graph contains a loop
            bool Graph::HasLoops()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if((*e)->IsLoop())
                        return true;
                return false;
            }

            /// \brief Test whether the graph is undirected and contains no loops
            bool Graph::IsMultigraph()
            {
                return IsUndirected() && !HasUndirectedLoops();
            }

            /// \brief Test whether the graph is directed and contains no directed loops
            bool Graph::IsMultiDigraph()
            {
                return IsDirected() && !HasDirectedLoops();
            }

            /// \brief Test whether the graph contains no loops
            bool Graph::IsMixedMultigraph()
            {
                return IsMixed() && !HasLoops();
            }

            bool Graph::IsSimpleGraph()
            {
                // ! HasParallelEdges
                return !IsHypergraph() && IsMultigraph();
            }

            bool Graph::IsSimpleDigraph()
            {
                // ! HasParallelEdges
                return !IsHypergraph() && IsMultiDigraph();
            }

        // @}


        bool Edge::IsEdge()
        {
            if(NumberOfConnections() == 2)
            {
                VertexEdgeConnectionIterator v1 = BeginConnections();
                VertexEdgeConnectionIterator v2 = v1+1;
                if((*v1)->GetDirection() == VertexEdgeConnection::Undirected
                    && (*v2)->GetDirection() == VertexEdgeConnection::Undirected)
                    return true;
            }
            return false;
        }

        bool Edge::IsArc()
        {
            if(NumberOfConnections() == 2)
            {
                VertexEdgeConnectionIterator v1 = BeginConnections();
                VertexEdgeConnectionIterator v2 = v1+1;
                if(
                     (
                         (*v1)->GetDirection() == VertexEdgeConnection::VertexToEdge
                      && (*v2)->GetDirection() == VertexEdgeConnection::EdgeToVertex
                     )
                  ||
                     (
                         (*v1)->GetDirection() == VertexEdgeConnection::EdgeToVertex
                      && (*v2)->GetDirection() == VertexEdgeConnection::VertexToEdge
                     )
                  )
                    return true;
            }
            return false;
        }

		/// \brief Test whether the EdgeIterator points to a hyperedge
		bool Edge::IsHyperedge()
		{
			return !IsEdge() && !IsArc();
		}

		/// \brief Test whether the graph contains at least one hyperedge
		bool Graph::IsHypergraph()
		{
			for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
				if((*e)->IsHyperedge())
					return true;
			return false;
		}

		bool Graph::IsUndirectedHypergraph()
		{
		    return IsHypergraph() && IsUndirected();
		}

		bool Graph::IsHyperDigraph()
		{
		    return IsHypergraph() && IsDirected();
		}

		bool Graph::IsMixedHypergraph()
		{
		    return IsHypergraph() && IsMixed();
		}

	// @}


	/// \defgroup attributemanagement ''Managing Attributes''
	// @{

        void Graph::AddVertexSet(VertexSet& V, string name)
        {
            for(VertexIterator v = BeginVertices(); v != EndVertices(); v++)
            {
                (*v)->RemoveAttribute(name);
                Attribute* attr = (*v)->AddAttribute(name, "bool");
                BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
                if(battr != NULL)
                    battr->Value = (V.contains(*v));
            }
        }

        void Graph::AddEdgeSet(EdgeSet& E, string name)
        {
            for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
            {
                (*e)->RemoveAttribute(name);
                Attribute* attr = (*e)->AddAttribute(name, "bool");
                BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
                if(battr != NULL)
                    battr->Value = (E.contains(*e));
            }
        }

        void Graph::AddVertexPartitioning(VertexPartitioning& Partitioning, string name)
        {
            for(VertexIterator v = BeginVertices(); v != EndVertices(); v++)
            {
                (*v)->RemoveAttribute(name);
                if(Partitioning.find(*v) != Partitioning.end())
                {
                    Attribute* attr = (*v)->AddAttribute(name, "int");
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        iattr->Value = Partitioning[*v];
                }
            }
        }

        void Graph::AddEdgePartitioning(EdgePartitioning& Partitioning, string name)
        {
            for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
            {
                (*e)->RemoveAttribute(name);
                if(Partitioning.find(*e) != Partitioning.end())
                {
                    Attribute* attr = (*e)->AddAttribute(name, "int");
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        iattr->Value = Partitioning[*e];
                }
            }
        }

        VertexPartitioning Graph::GetVertexPartitioning(string name)
        {
            VertexPartitioning result;
            if(name != "")
            {
                for(VertexIterator v = BeginVertices(); v != EndVertices(); v++)
                {
                    Attribute* attr = (*v)->GetAttribute(name);
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        result[*v] = iattr->Value;
                    else
                    {
                        BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
                        if(battr != NULL)
                            result[*v] = battr->Value ? 1 : 0;
                    }
                }
            }
            return result;
        }

        EdgePartitioning Graph::GetEdgePartitioning(string name)
        {
            EdgePartitioning result;
            if(name != "")
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                {
                    Attribute* attr = (*e)->GetAttribute(name);
                    IntAttribute* iattr = dynamic_cast<IntAttribute*>(attr);
                    if(iattr != NULL)
                        result[*e] = iattr->Value;
                    else
                    {
                        BoolAttribute* battr = dynamic_cast<BoolAttribute*>(attr);
                        if(battr != NULL)
                            result[*e] = battr->Value ? 1 : 0;
                    }
                }
            }
            return result;
        }

        void Graph::AddVertexWeighting(VertexWeighting& Weighting, string name)
        {
            for(VertexIterator v = BeginVertices(); v != EndVertices(); v++)
            {
                (*v)->RemoveAttribute(name);
                if(Weighting.find(*v) != Weighting.end())
                {
                    Attribute* attr = (*v)->AddAttribute(name, "float");
                    FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(attr);
                    if(fattr != NULL)
                        fattr->Value = Weighting[*v];
                }
            }
        }

        void Graph::AddEdgeWeighting(EdgeWeighting& Weighting, string name)
        {
            for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
            {
                (*e)->RemoveAttribute(name);
                if(Weighting.find(*e) != Weighting.end())
                {
                    Attribute* attr = (*e)->AddAttribute(name, "float");
                    FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(attr);
                    if(fattr != NULL)
                        fattr->Value = Weighting[*e];
                }
            }
        }

    // @}

	/// \defgroup vertexmanipulation ''Vertex Manipulation''
	// @{

		/// \brief Internal method: Add a new Vertex to the Graph, ID can be specified
		/// \return VertexIterator that points to the newly created vertex
		VertexIterator Graph::InternalAddVertex(int ID)
		{
			Vertex* v = new Vertex(this, ID);
			Vertices->insert(v);

			(*Vertex_ID_to_pointer)[v->GetID()] = v;
			return EndVertices() - 1;
		}

		VertexIterator Graph::AddVertex()
		{
            return InternalAddVertex(-1);
		}

		/// \brief Internal Method: Remove a vertex from the graph
		/// \param pv Vertex-Pointer that references the vertex which should be removed
		/// \param RemoveIncidentEdges Should incident edges be removed also or just disconnected from pv?
		void Graph::RemoveVertex(Vertex* pv, bool RemoveIncidentEdges)
		{
			if(RemoveIncidentEdges) // remove incident edges
			{
                while(pv->NumberOfConnections() > 0)
                    RemoveEdge((*(pv->BeginConnections()))->GetEdge());
			}
			else // or just disconnect incident edges from removed vertex
			{
                while(pv->NumberOfConnections() > 0)
                    pv->RemoveConnection(*(pv->BeginConnections()));
			}

			Vertex_ID_to_pointer->erase(pv->GetID());
			Vertices->erase(pv);
			delete(pv);
		}

		/// \brief Internal Method: Remove a vertex from the graph
		/// \param v VertexIterator that points to the vertex which should be removed
		/// \param RemoveIncidentEdges Should incident edges be removed also or just disconnected from pv?
        void Graph::RemoveVertex(VertexIterator v, bool RemoveIncidentEdges)
        {
            RemoveVertex(*v, RemoveIncidentEdges);
        }

        void Graph::operator-=(VertexIterator v)
        {
            RemoveVertex(v);
        }

        Graph Graph::operator-(VertexIterator v)
        {
            Graph result(*this);
            result.RemoveVertex(v);
            return result;
        }


        VertexIterator Graph::Fuse(VertexSet& FusedVertices)
        {
            VertexIterator result = AddVertex();
            Vertex* pResult = *result;

            // Set Coordinates of the resulting vertex (arithmetical middle of the
            // coordinates of the fused vertices)
            size_t Dimensionality = 0;
            for(VertexIterator v = FusedVertices.begin(); v != FusedVertices.end(); v++)
                if((*v)->GetCoordinates(0).size() > Dimensionality)
                    Dimensionality = (*v)->GetCoordinates(0).size();
            Coordinates ResultCoordinates(Dimensionality, 0.0f);
            for(VertexIterator v = FusedVertices.begin(); v != FusedVertices.end(); v++)
            {
                Coordinates vCoordinates = (*v)->GetCoordinates(0);
                for(int i = vCoordinates.size()-1; i >= 0; --i)
                    ResultCoordinates[i] += vCoordinates[i];
            }
            for(int i = ResultCoordinates.size()-1; i >= 0; --i)
                ResultCoordinates[i] /= FusedVertices.size();
            (*result)->SetCoordinates(ResultCoordinates);


            // Connect all affected edges to the new Vertex
            EdgeSet AffectedEdges;
            for(VertexIterator v = FusedVertices.begin(); v != FusedVertices.end(); v++)
                AffectedEdges += (*v)->CollectIncidentEdges(1,1,1);


            EdgeSet EdgesThatHaveBecomeLoops;
            for(EdgeIterator e = AffectedEdges.begin(); e != AffectedEdges.end(); e++)
            {
                bool HasBecomeLoop = true;
                for(VertexEdgeConnectionIterator i = (*e)->BeginConnections(); i != (*e)->EndConnections(); i++)
                {
                    if(FusedVertices.contains((*i)->GetVertex()))
                    {
                        // messes with internal data structures
                        (*i)->vertex = pResult;
                        pResult->Connections->push_back(*i);
                    }
                    else
                        HasBecomeLoop = false;
                }

                if(HasBecomeLoop)
                    EdgesThatHaveBecomeLoops.insert(*e);
            }

            // Disconnect the fused vertices from the affected edges and also remove them from the graph
            for(VertexIterator v = FusedVertices.begin(); v != FusedVertices.end(); v++)
            {
                Vertex* vp = *v;
                vp->Connections->clear(); // mess with the internal data structures to avoid that the edges are deleted when v is removed
                RemoveVertex(vp);
            }

            // Remove Edges that have become loops
            for(EdgeIterator e = EdgesThatHaveBecomeLoops.begin(); e != EdgesThatHaveBecomeLoops.end(); e++)
                RemoveEdge(*e);

            return result;
        }


	// @}


	/// \defgroup edgemanipulation ''Edge Manipulation''
	// @{

        VertexEdgeConnection* Vertex::AddConnection(Edge* edge, VertexEdgeConnection::Direction direction)
        {
            VertexEdgeConnection *conn = new VertexEdgeConnection();
            conn->vertex = this;
            conn->direction = direction;
            conn->edge = edge;
            this->Connections->push_back(conn);
            edge->Connections->push_back(conn);
            return conn;
        }

        VertexEdgeConnection* Edge::AddConnection(Vertex* vertex, VertexEdgeConnection::Direction direction)
        {
            return vertex->AddConnection(this, direction);
        }

        void Vertex::RemoveConnection(VertexEdgeConnection* conn)
        {
            this->Connections->erase(conn);
            conn->edge->Connections->erase(conn);
            delete(conn);
        }

        void Edge::RemoveConnection(VertexEdgeConnection* conn)
        {
            conn->vertex->RemoveConnection(conn);
        }

        /// \brief Adds a (Mixed Hyper)Edge to the Graph
        /// \param pVertices Pointers to Vertices that get undirected connections to the new (Hyper)Edge
        /// \param From Pointers to Vertices that get outgoing connections to the new (Hyper)Edge
        /// \param To Pointers to Vertices that get incoming connections from the new (Hyper)Edge
		EdgeIterator Graph::InternalAddEdge(Vertex* from, Vertex* to, bool Directed, int ID)
		{
			Edge* e = new Edge(this, ID);
			if(from != NULL)
                e->AddConnection(from, Directed ? VertexEdgeConnection::VertexToEdge : VertexEdgeConnection::Undirected);
            if(to != NULL)
                e->AddConnection(to, Directed ? VertexEdgeConnection::EdgeToVertex : VertexEdgeConnection::Undirected);

			Edges->push_back(e);
			(*Edge_ID_to_pointer)[e->GetID()] = e;
			return EndEdges() - 1;
		}

        /// \brief Adds an Edge to the Graph
        /// \param a, b References to the Vertices that are connected by the new Edge
        EdgeIterator Graph::AddEdge(Vertex* a, Vertex* b)
		{
			return InternalAddEdge(a, b, false, -1);
		}

        /// \brief Adds an Edge to the Graph
        /// \param a, b VertexIterators of the Vertices that are connected by the new Edge
        EdgeIterator Graph::AddEdge(VertexIterator a, VertexIterator b)
		{
			return InternalAddEdge(*a, *b, false, -1);
		}

        /// \brief Adds a Directed (Hyper)Edge to the Graph
        /// \param From Reference to the Vertex that gets an outgoing connection to the new Arc
        /// \param To Reference to the Vertex that gets an incomming connection from the new Arc
		EdgeIterator Graph::AddArc(Vertex* From, Vertex* To)
		{
			return InternalAddEdge(From, To, true, -1);
		}

        /// \brief Adds a Directed (Hyper)Edge to the Graph
        /// \param From VertexIterator of Vertex that gets an outgoing connection to the new Arc
        /// \param To VertexIterator of Vertex that gets an incomming connection from the new Arc
		EdgeIterator Graph::AddArc(VertexIterator From, VertexIterator To)
		{
			return InternalAddEdge(*From, *To, true, -1);
		}

        /// \brief Adds an undirected Loop-Edge to the Graph
        /// \param v VertexIterator of the Vertex that gets the loop
        EdgeIterator Graph::AddUndirectedLoop(VertexIterator v)
        {
            return InternalAddEdge(*v, *v, false, -1);
        }

        /// \brief Adds a directed Loop-Edge to the Graph
        /// \param v VertexIterator of the Vertex that gets the loop
        EdgeIterator Graph::AddDirectedLoop(VertexIterator v)
        {
            return InternalAddEdge(*v, *v, true, -1);
        }

        /// \brief Adds a Loose Edge (an edge with no connection to a vertex) to the Graph
        EdgeIterator Graph::AddLooseEdge()
        {
            return InternalAddEdge(NULL, NULL, false, -1);
        }


        /// \brief Internal Method: Removes an edge from the graph
        /// \param pe Pointer to the Edge that is removed
		void Graph::RemoveEdge(Edge* pe)
		{
			while(pe->NumberOfConnections() > 0)
                pe->RemoveConnection(*(pe->BeginConnections()));

			// remove from Edge_ID_to_pointer and Edges, free RAM
			Edge_ID_to_pointer->erase(pe->GetID());
			Edges->erase(pe);
			delete(pe);
		}

        /// \brief Removes an edge from the graph
        /// \param e EdgeIterator of the Edge that is removed
		void Graph::RemoveEdge(EdgeIterator e)
		{
			RemoveEdge(*e);
		}

        void Graph::operator-=(EdgeIterator e)
        {
            RemoveEdge(*e);
        }

        Graph Graph::operator-(EdgeIterator e)
        {
            Graph result(*this);
            result.RemoveEdge(*e);
            return result;
        }


	// @}


	/// \defgroup iteratorcreation ''Creating Iterators''
	// @{

		/// \defgroup graphsetiterators ''Creating Iterators for the sets of a Graph''
		// @{

			/// \brief <i>begin</i>-Iterator for the set of all vertices in the graph
			VertexIterator Graph::BeginVertices()
			{
				return Vertices->begin();
			}

			/// \brief <i>begin</i>-Iterator for the set of all vertices in the graph
			ConstVertexIterator Graph::BeginVertices() const
			{
				return Vertices->begin();
			}

			/// \brief <i>end</i>-Iterator for the set of all vertices in the graph
			VertexIterator Graph::EndVertices()
			{
				return Vertices->end();
			}

			/// \brief <i>end</i>-Iterator for the set of all vertices in the graph
			ConstVertexIterator Graph::EndVertices() const
			{
				return Vertices->end();
			}

			/// \brief <i>begin</i>-Iterator for the set of all edges in the graph
			EdgeIterator Graph::BeginEdges()
			{
                return Edges->begin();
			}

			/// \brief <i>begin</i>-Iterator for the set of all edges in the graph
			ConstEdgeIterator Graph::BeginEdges() const
			{
                return Edges->begin();
			}

			/// \brief <i>end</i>-Iterator for the set of all edges in the graph
			EdgeIterator Graph::EndEdges()
			{
                return Edges->end();
			}

			/// \brief <i>end</i>-Iterator for the set of all edges in the graph
			ConstEdgeIterator Graph::EndEdges() const
			{
                return Edges->end();
			}

		// @}


		/// \defgroup vertexsetiterators ''Creating Iterators for the sets of a Vertex''
		// @{

			VertexEdgeConnectionIterator Vertex::BeginConnections()
			{
                return Connections->begin();
			}

			ConstVertexEdgeConnectionIterator Vertex::BeginConnections() const
			{
                return Connections->begin();
			}

			VertexEdgeConnectionIterator Vertex::EndConnections()
			{
                return Connections->end();
			}

			ConstVertexEdgeConnectionIterator Vertex::EndConnections() const
			{
                return Connections->end();
			}

			VertexEdgeConnectionIterator Edge::BeginConnections()
			{
                return Connections->begin();
			}

			ConstVertexEdgeConnectionIterator Edge::BeginConnections() const
			{
                return Connections->begin();
			}

			VertexEdgeConnectionIterator Edge::EndConnections()
			{
                return Connections->end();
			}

			ConstVertexEdgeConnectionIterator Edge::EndConnections() const
			{
                return Connections->end();
			}

		// @}


		/// \defgroup edgesetiterators ''Creating Iterators for the sets of an Edge''
		// @{

			/// \brief Iterator for the first incident vertex of an edge
			Vertex* Edge::From()
			{
			    if(NumberOfConnections() == 2)
			    {
                    VertexEdgeConnectionIterator a = BeginConnections();
                    VertexEdgeConnectionIterator b = a+1;

                    if(   (*a)->GetDirection() == VertexEdgeConnection::Undirected
                       && (*b)->GetDirection() == VertexEdgeConnection::Undirected)
                       return (*a)->GetVertex();

                    if(   (*a)->GetDirection() == VertexEdgeConnection::VertexToEdge
                       && (*b)->GetDirection() == VertexEdgeConnection::EdgeToVertex)
                       return (*a)->GetVertex();

                    if(   (*a)->GetDirection() == VertexEdgeConnection::EdgeToVertex
                       && (*b)->GetDirection() == VertexEdgeConnection::VertexToEdge)
                       return (*b)->GetVertex();
			    }
                throw "Edge::From doesn't work on hyperedges";
			}

			/// \brief Iterator for the second incident vertex of an edge (meant for regular edges, but works on hyperedges, too)
			Vertex* Edge::To()
			{
			    if(NumberOfConnections() == 2)
			    {
                    VertexEdgeConnectionIterator a = BeginConnections();
                    VertexEdgeConnectionIterator b = a+1;

                    if(   (*a)->GetDirection() == VertexEdgeConnection::Undirected
                       && (*b)->GetDirection() == VertexEdgeConnection::Undirected)
                       return (*b)->GetVertex();

                    if(   (*a)->GetDirection() == VertexEdgeConnection::VertexToEdge
                       && (*b)->GetDirection() == VertexEdgeConnection::EdgeToVertex)
                       return (*b)->GetVertex();

                    if(   (*a)->GetDirection() == VertexEdgeConnection::EdgeToVertex
                       && (*b)->GetDirection() == VertexEdgeConnection::VertexToEdge)
                       return (*a)->GetVertex();
			    }
			    throw "Edge::To doesn't work on hyperedges";
			}

		// @}

	// @}


	/// \defgroup accessors ''Accessor-Methods''
	// @{

        /// \brief Accessor-method for reading IDs
        int GraphObject::GetID() const
        {
            return ID;
        }

        string GraphObject::GetIDString() const
        {
            stringstream s;
            s << ID;
            return s.str();
        }

        /// \brief Accessor-method for reading labels
        string GraphObject::GetLabel(string name, string DefaultValue)
        {
            StringAttribute* sattr = dynamic_cast<StringAttribute*>(GetAttribute(name));
            if(sattr != NULL)
                return sattr->Value;
            return DefaultValue;
        }

        /// \brief Accessor-method for writing labels
        void GraphObject::SetLabel(string label, string name)
        {
            RemoveAttribute(name);
            StringAttribute* sattr = dynamic_cast<StringAttribute*>(AddAttribute(name, "string"));
            sattr->Value = label;
        }

        /// \brief Accessor-method for reading weights
        float GraphObject::GetWeight(string name, float DefaultValue)
        {
            FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(GetAttribute(name));
            if(fattr != NULL)
                return fattr->Value;
            return DefaultValue;
        }

        /// \brief Accessor-method for writing labels
        void GraphObject::SetWeight(float weight, string name)
        {
            RemoveAttribute(name);
            FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(AddAttribute(name, "float"));
            fattr->Value = weight;
        }

        Coordinates GraphObject::GetCoordinates(int minCount, string name)
        {
            Coordinates result;
            SeqAttribute* sattr = dynamic_cast<SeqAttribute*>(GetAttribute(name));
            if(sattr != NULL)
            {
                for(std::list<Attribute*>::iterator i = sattr->Value.begin(); i != sattr->Value.end(); i++)
                {
                    FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(*i);
                    if(fattr != NULL)
                    {
                        result.push_back(fattr->Value);
                    }
                    else
                    {
                        IntAttribute* iattr = dynamic_cast<IntAttribute*>(*i);
                        if(iattr != NULL)
                        {
                            result.push_back(iattr->Value);
                        }
                    }
                }
            }
            while(result.size() < minCount)
                result.push_back(0.0f);
            return result;
        }

        void GraphObject::SetCoordinates(Coordinates& coordinates, string name)
        {
            RemoveAttribute(name);
            SeqAttribute* sattr = dynamic_cast<SeqAttribute*>(AddAttribute(name, "seq"));
            for(Coordinates::const_iterator i = coordinates.begin(); i != coordinates.end(); i++)
            {
                FloatAttribute* fattr = new FloatAttribute();
                fattr->Value = *i;
                sattr->Value.push_back( fattr );
            }
        }

	// @}


	/// \defgroup streaming
	// @{


        bool GraphObject::LoadFromXml(OpenGraphtheory::XML::XML* xml)
        {
            /// assign attributes
			list<OpenGraphtheory::XML::XML*> attrs = xml->FindChildren("attr");
			for(list<OpenGraphtheory::XML::XML*>::iterator attr = attrs.begin(); attr != attrs.end(); attr++)
                attributes->Set(*attr);
            return true;
        }

        bool Vertex::LoadFromXml(OpenGraphtheory::XML::XML* xml)
        {
            return GraphObject::LoadFromXml(xml);
        }

        bool Edge::LoadFromXml(OpenGraphtheory::XML::XML* xml, map<string, Vertex*>& Vertex_XML_ID_to_pointer, bool DefaultDirected)
        {
            bool result = GraphObject::LoadFromXml(xml);

            if(xml->name == "edge")
            {
                string xmlIsdirected = xml->GetAttribute("isdirected", "");
                bool Directed = DefaultDirected;
                if(xmlIsdirected != "")
                    Directed = (xmlIsdirected == "true");

                string xmlFrom = xml->GetAttribute("from", "");
                map<string,Vertex*>::iterator from = Vertex_XML_ID_to_pointer.find(xmlFrom);
                string xmlTo = xml->GetAttribute("to", "");
                map<string,Vertex*>::iterator to = Vertex_XML_ID_to_pointer.find(xmlTo);
                if(from == Vertex_XML_ID_to_pointer.end() || to == Vertex_XML_ID_to_pointer.end())
                    return false;

                this->AddConnection(from->second, Directed ? VertexEdgeConnection::VertexToEdge : VertexEdgeConnection::Undirected);
                this->AddConnection(to->second, Directed ? VertexEdgeConnection::EdgeToVertex : VertexEdgeConnection::Undirected);
            }
            else if(xml->name == "rel")
            {
				/// collect incident vertices
				list<OpenGraphtheory::XML::XML*> relends = xml->FindChildren("relend");
				for(list<OpenGraphtheory::XML::XML*>::iterator relend = relends.begin(); relend != relends.end(); relend++)
				{
					string xmlTarget = xml->GetAttribute("target", ""); // this is NOT necessarily the ID, that it gets internally!
					if(xmlTarget == "") // illegal or no ID
						return false;

					map<string,Vertex*>::iterator target = Vertex_XML_ID_to_pointer.find(xmlTarget);
					// no vertex with that ID
					if(target == Vertex_XML_ID_to_pointer.end())
						return false;

                    string xmldirection = xml->GetAttribute("direction", "none");
                    VertexEdgeConnection::Direction direction = VertexEdgeConnection::Undirected;
                    if(xmldirection == "none")
                        direction = VertexEdgeConnection::Undirected;
                    else if(xmldirection == "in")
                        direction = VertexEdgeConnection::VertexToEdge;
                    else if(xmldirection == "out")
                        direction = VertexEdgeConnection::EdgeToVertex;
                    else
                        return false;

                    this->AddConnection(target->second, direction);
				}
            }
            return result;
        }

		/// \brief Traverse an XML-structure, create a graph from it and copy it to *this
		bool Graph::LoadFromXML(OpenGraphtheory::XML::XML* root)
		{
		    if(root == NULL)
                throw "Error loading XML Document";
		    list<OpenGraphtheory::XML::XML*> gxl = root->FindChildren("gxl");
			if(gxl.size() > 1)
				throw "XML Document must have exactly 1 top element \"gxl\"";
			if(gxl.size() < 1)
				throw "XML Document contains no element \"gxl\" (document possibly empty)";

		    map<string, Vertex*> Vertex_XML_ID_to_pointer;
		    map<Edge*, OpenGraphtheory::XML::XML*> EdgeOrigin;

			list<OpenGraphtheory::XML::XML*> XMLGraph = gxl.front()->FindChildren("graph");
			if(XMLGraph.size() != 1)
				throw "XML Document must have exactly 1 \"graph\" element below the top element \"gxl\"";

			OpenGraphtheory::XML::XML* pGraph = XMLGraph.front();

            bool DefaultDirected = true;
            string edgemode = pGraph->GetAttribute("edgemode", "defaultdirected");
            if(edgemode == "defaultdirected" || edgemode == "directed")
                DefaultDirected = true;
            else if(edgemode == "defaultundirected" || edgemode == "undirected")
                DefaultDirected = false;



			/// instantiate children from factory
			for(list<OpenGraphtheory::XML::XML_Element*>::iterator child = pGraph->children.begin(); child != pGraph->children.end(); child++)
			{
                OpenGraphtheory::XML::XML* xchild = dynamic_cast<OpenGraphtheory::XML::XML*>(*child);
                if(xchild == NULL)
                    continue;

                GraphObject* obj = GraphObject::GraphObjectFactory.Produce(xchild->name);

                Edge* edge = dynamic_cast<Edge*>(obj);
                if(edge != NULL)
                {
                    this->Edges->insert(edge);
                    edge->Owner = this;
                    EdgeOrigin[edge] = xchild;
                    // eobj->LoadFromXml is called later (after all vertices have been loaded), because
                    // edges may reference vertices that are declared later
                }
                else
                {
                    Vertex* vertex = dynamic_cast<Vertex*>(obj);
                    if(vertex != NULL)
                    {
                        string id = xchild->GetAttribute("id", "");
                        if(id == "")
                            return false; // illegal or no ID
                        map<string,Vertex*>::iterator j = Vertex_XML_ID_to_pointer.find(id);
                        if(j != Vertex_XML_ID_to_pointer.end())
                            return false; // same ID twice
                        Vertex_XML_ID_to_pointer[id] = vertex;

                        this->Vertices->insert(vertex);
                        vertex->Owner = this;
                        vertex->LoadFromXml(xchild);
                    }
                    else
                    {
                        delete obj; // object is neither a vertex nor an edge
                    }
                }
            }

            // now load the edge-properties (attributes etc)
            for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                (*e)->LoadFromXml(EdgeOrigin[*e], Vertex_XML_ID_to_pointer, DefaultDirected);

			return true;
		}


		/// \brief Load an XML-structure from an istream using istream& operator>>(istream&, XML&),
		/// then read the graph from the XML structure using bool Graph::LoadFromXML(XML* root)
		bool Graph::LoadFromStream(istream& is)
		{
			OpenGraphtheory::XML::XML *xml = new OpenGraphtheory::XML::XML;
			is >> (*xml);
			bool result = LoadFromXML(xml);
			delete xml;
			return result;
		}

		/// \brief Load the file as an istream and load the graph from it,
		/// using bool Graph::LoadFromStream(istream& is)
		void Graph::LoadFromFile(string filename)
		{
			ifstream is(filename.c_str());
            LoadFromStream(is);
		}

		istream& operator>>(istream& is, Graph& G)
		{
			G.LoadFromStream(is);
			return is;
		}


		list<Graph> Graph::LoadGraphsFromXML(OpenGraphtheory::XML::XML* root)
		{
			if(root->name == "Graph")
			{
				Graph G;
				if(G.LoadFromXML(root))
					return(list<Graph>(1,G));
				else
					return(list<Graph>(0));
			}

			if(root->name == "Graphs")
			{
				list<OpenGraphtheory::XML::XML*> XMLGraphs = root->FindChildren("Graph");
				list<Graph> result;
				for(list<OpenGraphtheory::XML::XML*>::iterator i = XMLGraphs.begin(); i != XMLGraphs.end(); i++)
				{
					Graph G;
					G.LoadFromXML(*i);
					result.push_back(G);
				}
				return result;
			}
			throw "Trying to load a Graph from illegal XML structure";
		}

		list<Graph> Graph::LoadGraphsFromStream(istream& is)
		{
			OpenGraphtheory::XML::XML* xml = new OpenGraphtheory::XML::XML;
			is >> *xml;
			list<Graph> result = LoadGraphsFromXML(xml);
			delete xml;
			return result;
		}




        void GraphObject::WriteToXml(OpenGraphtheory::XML::XML* xml, string IdPrefix)
        {
            stringstream s;
            s << IdPrefix << GetID();
            xml->AddAttribute("id", s.str());

            attributes->WriteToXml(xml);
        }

        void VertexEdgeConnection::WriteToXml(OpenGraphtheory::XML::XML* relend)
        {
            relend->name = "relend";

            // attribute "target"
            relend->AddAttribute("target", "v" + GetVertex()->GetIDString());

            // attribute "direction"
            string direction;
            switch(GetDirection())
            {
                case VertexEdgeConnection::Undirected: direction = "none"; break;
                case VertexEdgeConnection::EdgeToVertex: direction = "out"; break;
                case VertexEdgeConnection::VertexToEdge: direction = "in"; break;
                default:
                    throw "VertexEdgeConnection with illegal \"direction\" value";
            }
            relend->AddAttribute("direction", direction);
        }

        void Vertex::WriteToXml(OpenGraphtheory::XML::XML* node)
        {
            node->name = "node";
            GraphObject::WriteToXml(node, "v");
        }

        void Edge::WriteToXml(OpenGraphtheory::XML::XML* edge)
        {
            GraphObject::WriteToXml(edge, "e");

            if(!IsHyperedge())
            {
                edge->name = "edge";

                // attribute "from"
                edge->AddAttribute("from", "v" + From()->GetIDString());
                // attribute "to"
                edge->AddAttribute("to", "v" + To()->GetIDString());
                // attribute "isdirected"
                edge->AddAttribute("isdirected", IsUndirected() ? "false" : "true");
            }
            else
            {
                edge->name = "rel";

                // children "relend"
                for(VertexEdgeConnectionIterator j = Connections->begin(); j != Connections->end(); j++)
                {
                    OpenGraphtheory::XML::XML* relend = new OpenGraphtheory::XML::XML("relend");
                    (*j)->WriteToXml(relend);
                    edge->AddChild(relend);
                }
            }

        }

		void Graph::WriteToStream(ostream& os, int indent)
		{
            // root tag "gxl"
			OpenGraphtheory::XML::XML* gxl = new OpenGraphtheory::XML::XML("gxl");
			gxl->AddChild(new OpenGraphtheory::XML::XML_Comment("www.Open-Graphtheory.org"));
			gxl->AddAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");


            // child tag "graph"
			OpenGraphtheory::XML::XML* graph = new OpenGraphtheory::XML::XML("graph");
			graph->AddAttribute("edgeids", "true");
			graph->AddAttribute("edgemode", "defaultundirected");
			graph->AddAttribute("hypergraph", "true");

            // vertices
			for(VertexIterator i = BeginVertices(); i != EndVertices(); i++)
			{
                OpenGraphtheory::XML::XML* node = new OpenGraphtheory::XML::XML("node");
                (*i)->WriteToXml(node);
                graph->AddChild(node);
            }

            // edges
			for(EdgeIterator i = BeginEdges(); i != EndEdges(); i++)
			{
                OpenGraphtheory::XML::XML* edge = new OpenGraphtheory::XML::XML("edge");
                (*i)->WriteToXml(edge);
                graph->AddChild(edge);
            }

			// arbitrary attributes
            attributes->WriteToXml(graph);


            gxl->AddChild(graph);

			os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			os << "<!DOCTYPE gxl SYSTEM \"http://www.gupro.de/GXL/gxl-1.0.dtd\">\n";
            gxl->WriteToStream(os, 0);
            delete gxl;
		}

		void Graph::SaveToFile(string filename)
		{
			ofstream os(filename.c_str());
			WriteToStream(os);
		}

		ostream& operator<<(ostream& os, Graph& G)
		{
			G.WriteToStream(os);
			return os;
		}

	// @}


} // namespace OpenGraphtheory

