
#include "../Headers/opengt.h"

using namespace std;

namespace OpenGraphtheory
{

	int Graph::Vertex_IDs = 0;
	int Graph::Edge_IDs = 0;


	Factory<GraphObject> GraphObject::GraphObjectFactory;
    FactoryRegistrator<GraphObject> Graph::GraphRegistrator(
        &GraphObject::GraphObjectFactory, "graph", new DefaultInstantiator<GraphObject, Graph>("graph", "", ""));
    FactoryRegistrator<GraphObject> Vertex::VertexRegistrator(
        &GraphObject::GraphObjectFactory, "node", new DefaultInstantiator<GraphObject, Vertex>("node", "", ""));
    FactoryRegistrator<GraphObject> Edge::EdgeRegistrator(
        &GraphObject::GraphObjectFactory, "edge", new DefaultInstantiator<GraphObject, Graph>("edge", "", ""));
    FactoryRegistrator<GraphObject> Edge::RelRegistrator(
        &GraphObject::GraphObjectFactory, "rel", new DefaultInstantiator<GraphObject, Graph>("rel", "", ""));


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
		Vertex::Vertex(Graph* owner)
            : GraphObject()
		{
			Owner = owner;
		}

		/// \brief Vertex Destructor
		Vertex::~Vertex()
		{
		}


		/// \brief Edge Constructor (protected, accessible for class "Graph")
		Edge::Edge(Graph* owner)
            : GraphObject()
		{
			Owner = owner;
		}

		/// \brief Vertex Destructor
		Edge::~Edge()
		{
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
			Vertex_ID_to_pointer = new map<int, VertexIterator>;
			Edge_ID_to_pointer = new map<int, EdgeIterator>;

			for(int i = 0; i < size; i++)
				AddVertex();
		}

		/// \brief Graph Copy-Constructor
		Graph::Graph(const Graph& G)
            : GraphObject()
		{
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

			#error Owner der Vertices & Edges wird nicht umgesetzt
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
                VertexIterator vnew = InternalAddVertex((*v)->ID);
                vnew.attributes = *((*v)->attributes);
            }

            /// copy edges
            for(ConstEdgeIterator e = G.BeginEdges(); e != G.EndEdges(); e++)
            {
                EdgeIterator ne = InternalAddEdge(NULL, NULL, true, (*e)->GetID());
                for(ConstVertexEdgeConnectionIterator conn = (*e)->BeginConnections(); conn != (*e)->EndConnections(); conn++)
                {
                    (*ne)->AddConnection(Vertex_ID_to_pointer[(*conn)->GetVertex()->GetID()], (*conn)->GetDirection());
                }
                (*ne)->attributes = *((*e)->attributes);
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


	/// \defgroup basictests ''Testing basic properties''
	// @{


        /// \defgroup adjacenceincidence ''Adjacence, Incidence''
        // @{

            /// \brief Test whether the Vertex is adjacent to the given vertex
            bool Vertex::Adjacent(const Vertex* to) const
            {
                for(ConstVertexEdgeConnectionIterator it = Connections.begin(); it != Connections.end(); it++)
                    if((*it)->GetEdge()->Incident(to))
                        return true;
                return false;
            }

            bool Vertex::UnderlyingAdjacent(const Vertex* to) const
            {
                for(VertexEdgeConnectionSet::const_iterator it = Connections.begin(); it != Connections.end(); it++)
                    if((*it)->Incident(to,1,1,1))
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
                    VertexEdgeConnection v2edirection = (*v2e)->GetDirection();
                    if(!(
                        ((UndirectedToUndirected || UndirectedToNegative || UndirectedToPositive) && v2edirection == VertexEdgeConnection::Undirected)
                        ||((PositiveToUndirected || PositiveToNegative || PositiveToPositive) && v2edirection == VertexEdgeConnection::VertexToEdge)
                        ||((NegativeToUndirected || NegativeToNegative || NegativeToPositive) && v2edirection == VertexEdgeConnection::EdgeToVertex)
                    ))
                        continue;
                    Edge* e = (*v2e)->GetEdge();

                    for(VertexEdgeConnectionIterator e2v = e->BeginConnections(); e2v != e->EndConnections(); e2v++)
                    {
                        VertexEdgeConnection e2vdirection = (*e2v)->GetDirection();

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
                for(VertexEdgeConnectionIterator c = Connections.begin(); c != Connections.end(); c++)
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




            Edge* Vertex::GetEdge(Vertex* to,
                    bool UndirectedToUndirected, bool UndirectedToPositive, bool UndirectedToNegative,
                    bool PositiveToUndirected,   bool PositiveToPositive,   bool PositiveToNegative,
                    bool NegativeToUndirected,   bool NegativeToPositive,   bool NegativeToNegative)

            {
                if(UndirectedToUndirected || UndirectedToPositive || UndirectedToNegative)
                    for(EdgeIterator e = BeginIncidentEdges(); e != EndIncidentEdges(); e++)
                        if(e.Incident(to,UndirectedToUndirected, UndirectedToPositive, UndirectedToNegative))
                            return e;

                if(PositiveToUndirected || PositiveToPositive || PositiveToNegative)
                    for(EdgeIterator e = BeginPositiveIncidentEdges(); e != EndPositiveIncidentEdges(); e++)
                        if(e.Incident(to,PositiveToUndirected, PositiveToPositive, PositiveToNegative))
                            return e;

                if(NegativeToUndirected || NegativeToPositive || NegativeToNegative)
                    for(EdgeIterator e = BeginNegativeIncidentEdges(); e != EndNegativeIncidentEdges(); e++)
                        if(e.Incident(to,NegativeToUndirected, NegativeToPositive, NegativeToNegative))
                            return e;

                return Owner->EndEdges();
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
            bool EdgeIterator::IsDirectedLoop()
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
            bool EdgeIterator::IsLoop()
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
                if(Partitioning.find(e) != Partitioning.end())
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

        void Graph::AddVertexWeighting(VertexWeighting Weighting, string name)
        {
            for(VertexIterator v = BeginVertices(); v != EndVertices(); v++)
            {
                (*v)->RemoveAttribute(name);
                if(Weight.find(*v) != Weight.end())
                {
                    Attribute* attr = (*v)->AddAttribute(name, "float");
                    FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(attr);
                    if(fattr != NULL)
                        fattr->Value = Weight[*v];
                }
            }
        }

        void Graph::AddEdgeWeighting(EdgeWeighting Weighting, string name)
        {
            for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
            {
                (*e)->RemoveAttribute(name);
                if(Weight.find(*e) != Weight.end())
                {
                    Attribute* attr = (*e)->AddAttribute(name, "float");
                    FloatAttribute* fattr = dynamic_cast<FloatAttribute*>(attr);
                    if(fattr != NULL)
                        fattr->Value = Weight[*e];
                }
            }
        }

    // @}

	/// \defgroup vertexmanipulation ''Vertex Manipulation''
	// @{

		/// \brief Internal method: Add a new Vertex to the Graph, ID can be specified
		/// \return VertexIterator that points to the newly created vertex
		VertexIterator Graph::InternalAddVertex(std::vector<float> coordinates, string label, float weight, int ID)
		{
			Vertex* v = new Vertex(this, coordinates, label, weight);

			/// Register ID
			if(ID <= 0)
				ID = ++Vertex_IDs;
			v->ID = ID;
			Vertex_ID_to_pointer[ID] = v;

			/// Add Vertex to set "Vertices"
			Vertices.push_back(v);
			list<Vertex*>::iterator result = Vertices.end();
			return VertexIterator(this, ID, &Vertices, --result);
		}

		/// \brief Add a new Vertex to the graph
		/// \return VertexIterator that points to the newly created instance
		VertexIterator Graph::AddVertex(string label, float weight)
		{
		    vector<float> coordinates;
			return InternalAddVertex(coordinates,label,weight, -1);
		}

		VertexIterator Graph::AddVertex(float x, string label, float weight)
		{
		    vector<float> coordinates;
		    coordinates.push_back(x);
			return InternalAddVertex(coordinates,label,weight, -1);
		}

		/// \brief Add a new Vertex to the graph
		/// \return VertexIterator that points to the newly created instance
		VertexIterator Graph::AddVertex(float x, float y, string label, float weight)
		{
		    vector<float> coordinates;
		    coordinates.push_back(x);
		    coordinates.push_back(y);
			return InternalAddVertex(coordinates,label,weight, -1);
		}

		/// \brief Add a new Vertex to the graph
		/// \return VertexIterator that points to the newly created instance
		VertexIterator Graph::AddVertex(float x, float y, float z, string label, float weight)
		{
		    vector<float> coordinates;
		    coordinates.push_back(x);
		    coordinates.push_back(y);
		    coordinates.push_back(z);
			return InternalAddVertex(coordinates,label,weight, -1);
		}

		/// \brief Internal Method: Remove a vertex from the graph
		/// \param pv Vertex-Pointer that references the vertex which should be removed
		/// \param RemoveIncidentEdges Should incident edges be removed also or just disconnected from pv?
		void Graph::RemoveVertex(Vertex* pv, bool RemoveIncidentEdges)
		{
			if(RemoveIncidentEdges) // remove incident edges
			{
                while(pv->IncidentEdges.size() > 0)
                    RemoveEdge(pv->IncidentEdges.front());
                while(pv->PositiveIncidentEdges.size() > 0)
                    RemoveEdge(pv->PositiveIncidentEdges.front());
                while(pv->NegativeIncidentEdges.size() > 0)
                    RemoveEdge(pv->NegativeIncidentEdges.front());
			}
			else // or just disconnect incident edges from removed vertex
			{
                while(pv->IncidentEdges.size() > 0)
                    pv->IncidentEdges.front()->RemoveUndirectedConnection(pv);
                while(pv->PositiveIncidentEdges.size() > 0)
                    pv->PositiveIncidentEdges.front()->RemoveOutgoingConnection(pv);
                while(pv->NegativeIncidentEdges.size() > 0)
                    pv->NegativeIncidentEdges.front()->RemoveIncomingConnection(pv);
			}

			Vertex_ID_to_pointer.erase(pv->ID);
			Vertices.remove(pv);
			delete(pv);
		}

		/// \brief Internal Method: Remove a vertex from the graph
		/// \param v VertexIterator that points to the vertex which should be removed
		/// \param RemoveIncidentEdges Should incident edges be removed also or just disconnected from pv?
        void Graph::RemoveVertex(VertexIterator v, bool RemoveIncidentEdges)
        {
            RemoveVertex(VertexIteratorToPointer(v), RemoveIncidentEdges);
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


        VertexIterator Graph::Fuse(set<VertexIterator> Vertices)
        {
            VertexIterator result = AddVertex();
            Vertex* pResult = VertexIteratorToPointer(result);
            set<Vertex*> pVertices;
            for(set<VertexIterator>::iterator v = Vertices.begin(); v != Vertices.end(); v++)
                pVertices.insert(VertexIteratorToPointer(*v));


            // Set Coordinates of the resulting vertex (arithmetical middle of the
            // coordinates of the fused vertices)
            unsigned int Dimensionality = 0;
            for(std::set<VertexIterator>::iterator v = Vertices.begin(); v != Vertices.end(); v++)
                if(v->GetCoordinates().size() > Dimensionality)
                    Dimensionality = v->GetCoordinates().size();
            std::vector<float> ResultCoordinates(Dimensionality, 0.0f);
            for(std::set<VertexIterator>::iterator v = Vertices.begin(); v != Vertices.end(); v++)
            {
                std::vector<float> vCoordinates = v->GetCoordinates();
                for(int i = vCoordinates.size()-1; i >= 0; --i)
                    ResultCoordinates[i] += vCoordinates[i];
            }
            for(int i = ResultCoordinates.size()-1; i >= 0; --i)
                ResultCoordinates[i] /= Vertices.size();
            result.SetCoordinates(ResultCoordinates);


            // Connect all affected edges to the new Vertex
            set<EdgeIterator> EdgesThatHaveBecomeLoops;
            set<EdgeIterator> AffectedEdges;
            for(set<VertexIterator>::iterator v = Vertices.begin(); v != Vertices.end(); v++)
            {
                VertexIterator vi = *v;
                set<EdgeIterator> Incident = vi.CollectIncidentEdges(1,1,1);
                AffectedEdges.insert(Incident.begin(), Incident.end());
            }

            for(set<EdgeIterator>::iterator e = AffectedEdges.begin(); e != AffectedEdges.end(); e++)
            {
                bool HasBecomeLoop = true;
                Edge* pe = EdgeIteratorToPointer(*e);
                for(list<Vertex*>::iterator i = pe->IncidentVertices.begin(); i != pe->IncidentVertices.end(); i++)
                {
                    if(pVertices.find(*i) != pVertices.end())
                    {
                        *i = pResult;
                        pResult->IncidentEdges.push_back(pe);
                    }
                    else
                        HasBecomeLoop = false;
                }
                for(list<Vertex*>::iterator i = pe->NegativeIncidentVertices.begin(); i != pe->NegativeIncidentVertices.end(); i++)
                {
                    if(pVertices.find(*i) != pVertices.end())
                    {
                        *i = pResult;
                        pResult->PositiveIncidentEdges.push_back(pe);
                    }
                    else
                        HasBecomeLoop = false;
                }
                for(list<Vertex*>::iterator i = pe->PositiveIncidentVertices.begin(); i != pe->PositiveIncidentVertices.end(); i++)
                {
                    if(pVertices.find(*i) != pVertices.end())
                    {
                        *i = pResult;
                        pResult->NegativeIncidentEdges.push_back(pe);
                    }
                    else
                        HasBecomeLoop = false;
                }

                if(HasBecomeLoop)
                    EdgesThatHaveBecomeLoops.insert(*e);
            }

            // Disconnect the fused vertices from the affected edges and also remove them from the graph
            for(set<VertexIterator>::iterator v = Vertices.begin(); v != Vertices.end(); v++)
            {
                Vertex* vp = VertexIteratorToPointer(*v);
                vp->IncidentEdges.clear();
                vp->NegativeIncidentEdges.clear();
                vp->PositiveIncidentEdges.clear();

                RemoveVertex(*v);
            }

            // Remove Edges that have become loops
            for(set<EdgeIterator>::iterator e = EdgesThatHaveBecomeLoops.begin(); e != EdgesThatHaveBecomeLoops.end(); e++)
                RemoveEdge(*e);

            return result;
        }


	// @}


	/// \defgroup edgemanipulation ''Edge Manipulation''
	// @{

        VertexEdgeConnectionIterator Vertex::AddConnection(Edge* edge, VertexEdgeConnection::Direction direction)
        {
            VertexEdgeConnection *conn = new VertexEdgeConnection();
            conn->vertex = this;
            conn->direction = direction;
            conn->edge = edge;
            this->Connections->push_back(conn);
            edge->Connections->push_back(conn);
            return EndConnections() - 1;
        }

        VertexEdgeConnectionIterator Edge::AddConnection(Vertex* vertex, VertexEdgeConnection::Direction direction)
        {
            return vertex->AddConnection(this, direction);
        }

        /// \brief Adds a (Mixed Hyper)Edge to the Graph
        /// \param pVertices Pointers to Vertices that get undirected connections to the new (Hyper)Edge
        /// \param From Pointers to Vertices that get outgoing connections to the new (Hyper)Edge
        /// \param To Pointers to Vertices that get incoming connections from the new (Hyper)Edge
		EdgeIterator Graph::InternalAddEdge(const Vertex* from, const Vertex* to, bool Directed, int ID)
		{
			Edge* e = new Edge(this);
			if(from != NULL)
                e->AddConnection(from, Directed ? VertexEdgeConnection::VertexToEdge : VertexEdgeConnection::Undirected);
            if(to != NULL)
                e->AddConnection(to, Directed ? VertexEdgeConnection::EdgeToVertex : VertexEdgeConnection::Undirected);

            /// Register ID
			if(ID <= 0)
				ID = ++Edge_IDs;
			e->ID = ID;
			Edge_ID_to_pointer[ID] = e;

            /// Add Edge to list "Edges"
			Edges.push_back(e);
			return EndEdges() - 1;
		}

        /// \brief Adds an Edge to the Graph
        /// \param a, b VertexIterators of the Vertices that are connected by the new Edge
        EdgeIterator Graph::AddEdge(VertexIterator a, VertexIterator b)
		{
			return InternalAddEdge(*a, *b, false, -1);
		}

        /// \brief Adds a Directed (Hyper)Edge to the Graph
        /// \param From VertexIterators of Vertices that get outgoing connections to the new (Hyper)Edge
        /// \param To VertexIteratirs of Vertices that get incoming connections from the new (Hyper)Edge
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
        EdgeIterator Graph::AddEdge()
        {
            return InternalAddEdge(NULL, NULL, false, -1)
        }


        /// \brief Internal Method: Removes an edge from the graph
        /// \param pe Pointer to the Edge that is removed
		void Graph::RemoveEdge(Edge* pe)
		{
			while(pe->IncidentVertices.size() > 0)
                pe->RemoveUndirectedConnection(pe->IncidentVertices.front());
			while(pe->PositiveIncidentVertices.size() > 0)
                pe->RemoveOutgoingConnection(pe->PositiveIncidentVertices.front());
			while(pe->NegativeIncidentVertices.size() > 0)
                pe->RemoveIncomingConnection(pe->NegativeIncidentVertices.front());

			// remove from Edge_ID_to_pointer and Edges, free RAM
			Edge_ID_to_pointer.erase(pe->ID);
			Edges.remove(pe);
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
				return Vertices == NULL ? NULL : Vertices->begin();
			}

			/// \brief <i>begin</i>-Iterator for the set of all vertices in the graph
			ConstVertexIterator Graph::BeginVertices() const
			{
				return Vertices == NULL ? NULL : Vertices->begin();
			}

			/// \brief <i>end</i>-Iterator for the set of all vertices in the graph
			VertexIterator Graph::EndVertices()
			{
				return Vertices == NULL ? NULL : Vertices->end();
			}

			/// \brief <i>end</i>-Iterator for the set of all vertices in the graph
			ConstVertexIterator Graph::EndVertices() const
			{
				return Vertices == NULL ? NULL : Vertices->end();
			}

			/// \brief <i>begin</i>-Iterator for the set of all edges in the graph
			EdgeIterator Graph::BeginEdges()
			{
                return Edges == NULL ? NULL : Edges->begin();
			}

			/// \brief <i>end</i>-Iterator for the set of all edges in the graph
			EdgeIterator Graph::EndEdges()
			{
                return Edges == NULL ? NULL : Edges->end();
			}

		// @}


		/// \defgroup vertexsetiterators ''Creating Iterators for the sets of a Vertex''
		// @{

			VertexEdgeConnectionIterator Vertex::BeginConnections()
			{
                return Connections->begin();
			}

			VertexEdgeConnectionIterator Vertex::EndConnections()
			{
                return Connections->end();
			}

			VertexEdgeConnectionIterator Edge::BeginConnections()
			{
                return Connections->begin();
			}

			VertexEdgeConnectionIterator Edge::EndConnections()
			{
                return Connections->end();
			}

		// @}


		/// \defgroup edgesetiterators ''Creating Iterators for the sets of an Edge''
		// @{

			/// \brief Iterator for the first incident vertex of an edge
			Vertex* Edge::From()
			{
			    if(e->IncidentVertices.size() == 2
                    && e->NegativeIncidentVertices.size() == 0
                    && e->PositiveIncidentVertices.size() == 0)
                    return BeginIncidentVertices();
			    if(e->IncidentVertices.size() == 0
                    && e->NegativeIncidentVertices.size() == 1
                    && e->PositiveIncidentVertices.size() == 1)
                    return BeginNegativeIncidentVertices();
                throw "EdgeIterator::From doesn't work on hyperedges";
			}

			/// \brief Iterator for the second incident vertex of an edge (meant for regular edges, but works on hyperedges, too)
			Vertex* Edge::To()
			{
			    Edge* e = *position;
			    if(e->IncidentVertices.size() == 2
                    && e->NegativeIncidentVertices.size() == 0
                    && e->PositiveIncidentVertices.size() == 0)
                    {
                        VertexIterator result = BeginIncidentVertices();
                        result++;
                        return result;
                    }
			    if(e->IncidentVertices.size() == 0
                    && e->NegativeIncidentVertices.size() == 1
                    && e->PositiveIncidentVertices.size() == 1)
                    return BeginPositiveIncidentVertices();
                throw "EdgeIterator::To doesn't work on hyperedges";

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
        string GraphObject::GetLabel(string name, string DefaultValue) const
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
        float GraphObject::GetWeight(string name, float DefaultValue) const
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

        std::vector<float> VertexIterator::GetCoordinates() const
        {
            return (*position)->Coordinates;
        }

        void VertexIterator::SetCoordinates(std::vector<float>& coordinates)
        {
            (*position)->Coordinates = coordinates;
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
        }

        bool Vertex::LoadFromXml(OpenGraphtheory::XML::XML* xml)
        {
            GraphObject::LoadFromXml(xml);
        }

        bool Edge::LoadFromXml(OpenGraphtheory::XML::XML* xml, bool DefaultDirected)
        {
            GraphObject::LoadFromXml(xml);

            if(xml->name == "edge")
            {
                string xmlIsdirected = (*edge)->GetAttribute("isdirected", "");
                bool Directed = DefaultDirected;
                if(xmlIsdirected != "")
                    Directed = (xmlIsdirected == "true");

                string xmlFrom = (*edge)->GetAttribute("from", "");
                map<string,Vertex*>::iterator from = Vertex_XML_ID_to_pointer.find(xmlFrom);
                string xmlTo = (*edge)->GetAttribute("to", "");
                map<string,Vertex*>::iterator to = Vertex_XML_ID_to_pointer.find(xmlTo);
                if(from == Vertex_XML_ID_to_pointer.end() || to == Vertex_XML_ID_to_pointer.end())
                    return false;

                this->AddConnection(*(from->second), Directed ? VertexEdgeConnection::VertexToEdge : VertexEdgeConnection::Undirected);
                this->AddConnection(*(to->second), Directed ? VertexEdgeConnection::EdgeToVertex : VertexEdgeConnection::Undirected);
            }
            else if(xml->name == "rel")
            {
				/// collect incident vertices
				list<OpenGraphtheory::XML::XML*> relends = (*rel)->FindChildren("relend");
				for(list<OpenGraphtheory::XML::XML*>::iterator relend = relends.begin(); relend != relends.end(); relend++)
				{
					string xmlTarget = (*relend)->GetAttribute("target", ""); // this is NOT necessarily the ID, that it gets internally!
					if(xmlTarget == "") // illegal or no ID
						return false;

					map<string,VertexIterator*>::iterator target = Vertex_XML_ID_to_pointer.find(xmlTarget);
					// no vertex with that ID
					if(target == Vertex_XML_ID_to_pointer.end())
						return false;

                    string xmldirection = (*relend)->GetAttribute("direction", "none");
                    VertexEdgeConnection::Direction direction = VertexEdgeConnection::Undirected;
                    if(xmldirection == "none")
                        direction = VertexEdgeConnection::Undirected;
                    else if(xmldirection == "in")
                        direction = VertexEdgeConnection::VertexToEdge;
                    else if(xmldirection == "out")
                        direction = VertexEdgeConnection::EdgeToVertex;
                    else
                        return false;

                    this->AddConnection(*(target->second), direction);
				}
            }
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
                        map<string,VertexIterator*>::iterator j = Vertex_XML_ID_to_pointer.find(id);
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
            for(EdgeIterator e = this->Edges->begin(); e != this->Edges->end(); e++)
                (*e)->LoadFromXml(EdgeOrigin[*e], Vertex_XML_ID_to_pointer, DefaultUndirected);

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
            s << "IdPrefix" << GetID();
            xml->AddAttribute("id", s.str());

            attributes->WriteToXml(xml);
        }

        void VertexEdgeConnection::WriteToXml(OpenGraphtheory::XML::XML* relend)
        {
            relend->name = "relend";

            // attribute "target"
            relend->AddAttribute("target", "v" + (*j)->GetVertex()->GetIDString());

            // attribute "direction"
            string direction;
            switch((*j)->GetVertex()->GetDirection())
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
                for(VertexEdgeConnectionIterator j = Connections.begin(); j != Connections.end(); j++)
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

