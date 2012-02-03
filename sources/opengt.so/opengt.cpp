
#include "opengt.h"

namespace OpenGraphtheory
{

	int Graph::Vertex_IDs = 0;
	int Graph::Edge_IDs = 0;


	/// \defgroup constructorsdestructors ''Constructors, Destructors and low-level Methods''
	// @{

		/// \brief Vertex Constructor (protected, accessible for class "Graph")
		Graph::Vertex::Vertex(Graph* owner, int x, int y, string label, float weight, void* tag)
		{
			Owner = owner;
			X = x;
			Y = y;
			Label = label;
			Weight = weight;
			Tag = tag;
		}

		/// \brief Edge Constructor (protected, accessible for class "Graph")
		Graph::Edge::Edge(Graph* owner, string label, float weight, void* tag)
		{
			Owner = owner;
			Label = label;
			Weight = weight;
			Tag = tag;
		}

		/// \brief Graph Destructor
		Graph::~Graph()
		{
			Clear();
		}

		/// \brief Graph Constructor, creates a graph with "size" vertices
		Graph::Graph(int size)
		{
			for(int i = 0; i < size; i++)
				AddVertex(-1, -1, "", 0, 0);
		}

		/// \brief Graph Copy-Constructor
		Graph::Graph(const Graph& G)
		{
			operator=(G);
		}



        /// \brief Operator to copy a graph
        void Graph::operator=(const Graph& G)
        {
            Clear();

            Label = G.GetLabel();
            ID = G.GetID();

            /// copy vertices
            for(list<Graph::Vertex*>::const_iterator v = G.Vertices.begin(); v != G.Vertices.end(); v++)
                InternalAddVertex((*v)->X, (*v)->Y, (*v)->Label, (*v)->Weight, (*v)->Tag, (*v)->ID);

            /// copy edges
            for(list<Graph::Edge*>::const_iterator e = G.Edges.begin(); e != G.Edges.end(); e++)
            {
                list<Graph::Vertex*> OwnIncident;
                for(list<Graph::Vertex*>::const_iterator j = (*e)->IncidentVertices.begin(); j != (*e)->IncidentVertices.end(); j++)
                    OwnIncident.push_back(Vertex_ID_to_pointer[(*j)->ID]);

                list<Graph::Vertex*> OwnPosIncident;
                for(list<Graph::Vertex*>::const_iterator j = (*e)->PositiveIncidentVertices.begin(); j != (*e)->PositiveIncidentVertices.end(); j++)
                    OwnPosIncident.push_back(Vertex_ID_to_pointer[(*j)->ID]);

                list<Graph::Vertex*> OwnNegIncident;
                for(list<Graph::Vertex*>::const_iterator j = (*e)->NegativeIncidentVertices.begin(); j != (*e)->NegativeIncidentVertices.end(); j++)
                    OwnNegIncident.push_back(Vertex_ID_to_pointer[(*j)->ID]);

                InternalAddEdge(OwnIncident, OwnPosIncident, OwnNegIncident, (*e)->Label, (*e)->Weight, (*e)->Tag, (*e)->ID);
            }
        }

        /// \brief Remove all vertices and edges
        void Graph::Clear()
        {
            while(Vertices.size() > 0)
                RemoveVertex(*(Vertices.begin()));
        }

        /// \brief Internal method: extract pointer to actual Vertex object from iterator
        Graph::Vertex* Graph::VertexIteratorToPointer(const Graph::VertexIterator& i)
        {
            if(i.Owner == this)
                return *(i.position);
            map<int,Vertex*>::iterator j = Vertex_ID_to_pointer.find(i.ID);
            if(j == Vertex_ID_to_pointer.end())
                throw "Tried to access a Vertex that is not in the Graph";
            return j->second;
        }

        /// \brief Internal method: extract pointer to actual Edge object from iterator
        Graph::Edge* Graph::EdgeIteratorToPointer(const Graph::EdgeIterator& i)
        {
            if(i.Owner == this)
                return *(i.position);
            map<int,Edge*>::iterator j = Edge_ID_to_pointer.find(i.ID);
            if(j == Edge_ID_to_pointer.end())
                throw "Tried to access an Edge that is not in the Graph";
            return j->second;
        }

	// @}


	/// \defgroup basictests ''Testing basic properties''
	// @{


        /// \defgroup adjacenceincidence ''Adjacence, Incidence''
        // @{

            /// \brief Test whether the Vertex is adjacent to the given vertex
            bool Graph::Vertex::Adjacent(const Graph::Vertex* to) const
            {
                for(list<Graph::Edge*>::const_iterator it = IncidentEdges.begin(); it != IncidentEdges.end(); it++)
                    if((*it)->Incident(to))
                        return true;
                return false;
            }

            /// \brief Test whether the Vertex is adjacent to the given vertex
            bool Graph::VertexIterator::Adjacent(const Graph::VertexIterator& to) const
            {
                Vertex* v = Owner->VertexIteratorToPointer(to);
                return (*position)->Adjacent(v);
            }

            /// \brief Test whether the Edge is incident to the given vertex
            bool Graph::Edge::Incident(const Graph::Vertex* to) const
            {
                for(list<Graph::Vertex*>::const_iterator it = IncidentVertices.begin(); it != IncidentVertices.end(); it++)
                    if(*it == to)
                        return true;
                return false;
            }

            /// \brief Test whether the Edge is incident to the given vertex
            bool Graph::EdgeIterator::Incident(const Graph::VertexIterator& to) const
            {
                Vertex* v = Owner->VertexIteratorToPointer(to);
                return (*position)->Incident(v);
            }

        // @}


        /// \defgroup directedundirected ''Directed, Undirected, Mixed''
        // @{

            /// \brief Test whether the edge is undirected (has no directed connections)
            bool Graph::EdgeIterator::IsUndirected()
            {
                Edge* e = *position;
                return e->PositiveIncidentVertices.size() == 0
                    && e->NegativeIncidentVertices.size() == 0;
            }

            /// \brief Test whether the graph has no directed edges
            bool Graph::IsUndirected()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if(!e.IsUndirected())
                        return false;
                return true;
            }

            /// \brief Test whether the edge is directed (has no undirected connections)
            bool Graph::EdgeIterator::IsDirected()
            {
                Edge* e = *position;
                return e->IncidentVertices.size() == 0;
            }

            /// \brief Test whether the graph has no undirected edges
            bool Graph::IsDirected()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if(!e.IsDirected())
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
            bool Graph::EdgeIterator::IsUndirectedLoop()
            {
                Edge* e = *position;
                if(e->IncidentVertices.size() == 2
                    && e->PositiveIncidentVertices.size() == 0
                    && e->NegativeIncidentVertices.size() == 0)
                    {
                        list<Graph::Vertex*>::iterator v1 = e->IncidentVertices.begin();
                        v1++;
                        return *v1 == *(e->IncidentVertices.begin());
                    }
                return false;
            }

            /// \brief Test whether the graph contains an undirected loop
            bool Graph::HasUndirectedLoops()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if(e.IsUndirectedLoop())
                        return true;
                return false;
            }

            /// \brief Test whether the Edge is a directed loop
            bool Graph::EdgeIterator::IsDirectedLoop()
            {
                Edge* e = *position;
                return (e->IncidentVertices.size() == 0
                    && e->PositiveIncidentVertices.size() == 1
                    && e->NegativeIncidentVertices.size() == 1
                    && *(e->PositiveIncidentVertices.begin()) == *(e->NegativeIncidentVertices.begin()));
            }

            /// \brief Test whether the graph contains a directed loop
            bool Graph::HasDirectedLoops()
            {
                for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if(e.IsDirectedLoop())
                        return true;
                return false;
            }

            /// \brief Test whether the Edge is a loop
            bool Graph::EdgeIterator::IsLoop()
            {
                return IsUndirectedLoop() || IsDirectedLoop();
            }

            /// \brief Test whether the graph contains a loop
            bool Graph::HasLoops()
            {
                for(Graph::EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
                    if(e.IsLoop())
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
                return IsMixed() &&!HasLoops();
            }

        // @}


		/// TODO: Test whether the graph
		bool Graph::IsSimpleGraph() {return false;}


        bool Graph::EdgeIterator::IsEdge()
        {
            return (*position)->IncidentVertices.size() == 2
				&& (*position)->PositiveIncidentVertices.size() == 0
				&& (*position)->NegativeIncidentVertices.size() == 0;
        }

        bool Graph::EdgeIterator::IsArc()
        {
            return (*position)->IncidentVertices.size() == 0
				&& (*position)->PositiveIncidentVertices.size() == 1
				&& (*position)->NegativeIncidentVertices.size() == 1;
        }

		/// \brief Test whether the EdgeIterator points to a hyperedge
		bool Graph::EdgeIterator::IsHyperedge()
		{
			if( (*position)->IncidentVertices.size() > 0 )
			{
				return (*position)->IncidentVertices.size() > 2
					|| (*position)->NegativeIncidentVertices.size() != 0
					|| (*position)->PositiveIncidentVertices.size() != 0;
			}
			else
			{
				return (*position)->NegativeIncidentVertices.size() != 1
					|| (*position)->PositiveIncidentVertices.size() != 1;
			}
		}

		/// \brief Test whether the graph contains at least one hyperedge
		bool Graph::IsHypergraph()
		{
			for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
				if(e.IsHyperedge())
					return true;

			return false;
		}

		bool Graph::IsHyperDigraph()
		{
		    return false;
		}

		bool Graph::IsMixedHypergraph()
		{
		    return false;
		}

	// @}


	/// \defgroup vertexmanipulation ''Vertex Manipulation''
	// @{

		/// \brief Internal method: Add a new Vertex to the Graph, ID can be specified
		/// \return VertexIterator that points to the newly created vertex
		Graph::VertexIterator Graph::InternalAddVertex(int x, int y, string label, float weight, void* tag, int ID)
		{
			Vertex* v = new Vertex(this, x, y, label, weight, tag);

			/// Register ID
			if(ID <= 0)
				ID = ++Vertex_IDs;
			v->ID = ID;
			Vertex_ID_to_pointer[ID] = v;

			/// Add Vertex to set "Vertices"
			Vertices.push_back(v);
			list<Graph::Vertex*>::iterator result = Vertices.end();
			return VertexIterator(this, ID, &Vertices, --result);
		}

		/// \brief Add a new Vertex to the graph
		/// \return VertexIterator that points to the newly created instance
		Graph::VertexIterator Graph::AddVertex(int x, int y, string label, float weight, void* tag)
		{
			return InternalAddVertex(x,y,label,weight, tag, -1);
		}

		/// \brief Internal Method: Remove a vertex from the graph
		/// \param pv Vertex-Pointer that references the vertex which should be removed
		/// \param RemoveIncidentEdges Should incident edges be removed also or just disconnected from pv?
		void Graph::RemoveVertex(Graph::Vertex* pv, bool RemoveIncidentEdges)
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
        void Graph::RemoveVertex(Graph::VertexIterator v, bool RemoveIncidentEdges)
        {
            RemoveVertex(VertexIteratorToPointer(v), RemoveIncidentEdges);
        }

	// @}


	/// \defgroup edgemanipulation ''Edge Manipulation''
	// @{


        /// \brief Adds an undirected connection between an edge and a vertex
        void Graph::Edge::AddUndirectedConnection(Graph::Vertex* v)
        {
            this->IncidentVertices.push_back(v);
            v->IncidentEdges.push_back(this);
        }

        /// \brief Adds a connection from a vertex to an edge
        void Graph::Edge::AddIncomingConnection(Graph::Vertex* v)
        {
            this->NegativeIncidentVertices.push_back(v);
            v->PositiveIncidentEdges.push_back(this);
        }

        /// \brief Adds a connection from an edge to a vertex
        void Graph::Edge::AddOutgoingConnection(Graph::Vertex* v)
        {
            this->PositiveIncidentVertices.push_back(v);
            v->NegativeIncidentEdges.push_back(this);
        }

        /// \brief Removes a connection from a vertex to an edge
        void Graph::Edge::RemoveUndirectedConnection(Graph::Vertex* v)
        {
            this->IncidentVertices.remove(v);
            v->IncidentEdges.remove(this);
        }

        /// \brief Removes a connection from a vertex to an edge
        void Graph::Edge::RemoveIncomingConnection(Graph::Vertex* v)
        {
            this->NegativeIncidentVertices.remove(v);
            v->PositiveIncidentEdges.remove(this);
        }

        /// \brief Removes a connection from an edge to a vertex
        void Graph::Edge::RemoveOutgoingConnection(Graph::Vertex* v)
        {
            this->PositiveIncidentVertices.remove(v);
            v->NegativeIncidentEdges.remove(this);
        }


        /// \brief Adds a (Mixed Hyper)Edge to the Graph
        /// \param pVertices Pointers to Vertices that get undirected connections to the new (Hyper)Edge
        /// \param From Pointers to Vertices that get outgoing connections to the new (Hyper)Edge
        /// \param To Pointers to Vertices that get incoming connections from the new (Hyper)Edge
		Graph::EdgeIterator Graph::InternalAddEdge(const list<Graph::Vertex*> &pVertices, const list<Graph::Vertex*> &From, const list<Graph::Vertex*> &To,
                                             string label, float weight, void* tag, int ID)
		{
			Edge* e = new Edge(this, label, weight, tag);

            /// Copy Vertex-Pointers into the lists of the edge
            for(list<Graph::Vertex*>::const_iterator it = pVertices.begin(); it != pVertices.end(); it++)
			    e->AddUndirectedConnection(*it);
            for(list<Graph::Vertex*>::const_iterator it = From.begin(); it != From.end(); it++)
			    e->AddIncomingConnection(*it);
            for(list<Graph::Vertex*>::const_iterator it = To.begin(); it != To.end(); it++)
			    e->AddOutgoingConnection(*it);

            /// Register ID
			if(ID <= 0)
				ID = ++Edge_IDs;
			e->ID = ID;
			Edge_ID_to_pointer[ID] = e;

            /// Add Edge to list "Edges"
			Edges.push_back(e);
			list<Graph::Edge*>::iterator result = Edges.end();
			return EdgeIterator(this, ID, &Edges, --result);
		}

        /// \brief Adds a (Mixed Hyper)Edge to the Graph
        /// \param vertices VertexIterators of Vertices that get undirected connections to the new (Hyper)Edge
        /// \param From VertexIterators of Vertices that get outgoing connections to the new (Hyper)Edge
        /// \param To VertexIteratirs of Vertices that get incoming connections from the new (Hyper)Edge
		Graph::EdgeIterator Graph::InternalAddEdge(const list<Graph::VertexIterator>* vertices, const list<Graph::VertexIterator>* From,
		                                           const list<Graph::VertexIterator>* To, string label, float weight, void* tag, int ID)
		{
			list<Vertex*> pVertices;
			if(vertices != NULL)
				for(list<VertexIterator>::const_iterator i = vertices->begin(); i != vertices->end(); i++)
					pVertices.push_back(VertexIteratorToPointer(*i));

			list<Vertex*> pFrom;
			if(From != NULL)
				for(list<VertexIterator>::const_iterator i = From->begin(); i != From->end(); i++)
					pFrom.push_back(VertexIteratorToPointer(*i));

			list<Vertex*> pTo;
			if(To != NULL)
				for(list<VertexIterator>::const_iterator i = To->begin(); i != To->end(); i++)
					pTo.push_back(VertexIteratorToPointer(*i));

			Graph::EdgeIterator result = InternalAddEdge(pVertices, pFrom, pTo, label, weight, tag, ID);
			return result;
		}

        /// \brief Adds a (Hyper)Edge to the Graph
        /// \param vertices VertexIterators of Vertices that are connected by the new (Hyper)Edge
		Graph::EdgeIterator Graph::InternalAddEdge(const list<Graph::VertexIterator>& vertices, string label, float weight, void* tag, int ID)
		{
			return InternalAddEdge(&vertices, NULL, NULL, label, weight, tag, ID);
		}

        /// \brief Adds an Edge to the Graph
        /// \param a, b VertexIterators of the Vertices that are connected by the new Edge
		Graph::EdgeIterator Graph::InternalAddEdge(Graph::VertexIterator a, Graph::VertexIterator b, string label, float weight, void* tag, int ID)
		{
			list<VertexIterator> s;
			s.push_back(a);
			s.push_back(b);
			return InternalAddEdge(&s, NULL, NULL, label, weight, tag, ID);
		}

        /// \brief Adds a Directed (Hyper)Edge to the Graph
        /// \param From VertexIterators of Vertices that get outgoing connections to the new (Hyper)Edge
        /// \param To VertexIteratirs of Vertices that get incoming connections from the new (Hyper)Edge
		Graph::EdgeIterator Graph::InternalAddArc(const list<Graph::VertexIterator>& From, const list<Graph::VertexIterator>& To, string label, float weight, void* tag, int ID)
		{
			return InternalAddEdge(NULL, &From, &To, label, weight, tag, ID);
		}

        /// \brief Adds a Directed Edge to the Graph
        /// \param From, To VertexIterators of Vertices that are connected by the new Edge
		Graph::EdgeIterator Graph::InternalAddArc(Graph::VertexIterator From, Graph::VertexIterator To, string label, float weight, void* tag, int ID)
		{
			list<VertexIterator> f;
			f.push_back(From);
			list<VertexIterator> t;
			t.push_back(To);
			return InternalAddEdge(NULL, &f, &t, label, weight, tag, ID);
		}



        /// \brief Adds a (Mixed Hyper)Edge to the Graph
        /// \param vertices VertexIterators of Vertices that get undirected connections to the new (Hyper)Edge
        /// \param From VertexIterators of Vertices that get outgoing connections to the new (Hyper)Edge
        /// \param To VertexIteratirs of Vertices that get incoming connections from the new (Hyper)Edge
		Graph::EdgeIterator Graph::AddEdge(const list<Graph::VertexIterator>& vertices, const list<Graph::VertexIterator>& from,
                                      const list<Graph::VertexIterator>& to, string label, float weight, void* tag)
		{
			return InternalAddEdge(&vertices, &from, &to, label, weight, tag, -1);
		}

		/// \brief Adds a (Hyper)Edge to the Graph
        /// \param vertices VertexIterators of Vertices that are connected by the new (Hyper)Edge
        Graph::EdgeIterator Graph::AddEdge(const list<Graph::VertexIterator>& vertices, string label, float weight, void* tag)
		{
			return InternalAddEdge(vertices, label, weight, tag, -1);
		}

        /// \brief Adds an Edge to the Graph
        /// \param a, b VertexIterators of the Vertices that are connected by the new Edge
        Graph::EdgeIterator Graph::AddEdge(Graph::VertexIterator a, Graph::VertexIterator b, string label, float weight, void* tag)
		{
			return InternalAddEdge(a, b, label, weight, tag, -1);
		}

        /// \brief Adds a Directed (Hyper)Edge to the Graph
        /// \param From VertexIterators of Vertices that get outgoing connections to the new (Hyper)Edge
        /// \param To VertexIteratirs of Vertices that get incoming connections from the new (Hyper)Edge
		Graph::EdgeIterator Graph::AddArc(const list<Graph::VertexIterator>& From, const list<Graph::VertexIterator>& To, string label, float weight, void* tag)
		{
			return InternalAddArc(From, To, label, weight, tag, -1);
		}

        /// \brief Adds a Directed Edge to the Graph
        /// \param From, To VertexIterators of Vertices that are connected by the new Edge
		Graph::EdgeIterator Graph::AddArc(Graph::VertexIterator From, Graph::VertexIterator To, string label, float weight, void* tag)
		{
			return InternalAddArc(From, To, label, weight, tag, -1);
		}

        /// \brief Adds an undirected Loop-Edge to the Graph
        /// \param v VertexIterator of the Vertex that gets the loop
        Graph::EdgeIterator Graph::AddLoop(Graph::VertexIterator v, string label, float weight, void* tag)
        {
            return InternalAddEdge(v,v, label, weight, tag, -1);
        }

        /// \brief Adds a directed Loop-Edge to the Graph
        /// \param v VertexIterator of the Vertex that gets the loop
        Graph::EdgeIterator Graph::AddDirectedLoop(Graph::VertexIterator v, string label, float weight, void* tag)
        {
            return InternalAddArc(v, v, label, weight, tag, -1);
        }

        /// \brief Adds a Loose Edge (an edge with no connection to a vertex) to the Graph
        Graph::EdgeIterator Graph::AddLooseEdge(string label, float weight, void* tag)
        {
            return InternalAddEdge(NULL, NULL, NULL, label, weight, tag, -1);
        }


        /// \brief Internal Method: Removes an edge from the graph
        /// \param pe Pointer to the Edge that is removed
		void Graph::RemoveEdge(Graph::Edge* pe)
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
		void Graph::RemoveEdge(Graph::EdgeIterator e)
		{
			RemoveEdge(EdgeIteratorToPointer(e));
		}

	// @}


	/// \defgroup vertexiteratormethods ''VertexIterator Methods''
	// @{

        Graph::VertexIterator::VertexIterator()
        {
            Owner = NULL;
            iterating = NULL;
            ID = -1;
        }

		Graph::VertexIterator::VertexIterator(Graph* owner, int id, list<Graph::Vertex*>* Iterating, list<Graph::Vertex*>::iterator position)
		{
			Owner = owner;
			Owner->VIterators.insert(this);
			ID = id;
			iterating = Iterating;
			this->position = position;
		}

		Graph::VertexIterator::VertexIterator(const Graph::VertexIterator& i)
		{
			ID = i.ID;
			Owner = i.Owner;
			iterating = i.iterating;
			position = i.position;
			// this is why no default-constructor is used
			if(Owner != NULL)
                Owner->VIterators.insert(this);
		}

		void Graph::VertexIterator::operator=(const Graph::VertexIterator& i)
		{
			ID = i.ID;
			iterating = i.iterating;
			position = i.position;
			// this is why no default-constructor is used
			if(Owner != i.Owner)
			{
			    if(Owner != NULL)
                    Owner->VIterators.erase(this);
				Owner = i.Owner;
				if(Owner != NULL)
                    Owner->VIterators.insert(this);
			}
		}

		Graph::VertexIterator::~VertexIterator()
		{
		    if(Owner != NULL)
                Owner->VIterators.erase(this);
		}

		void Graph::VertexIterator::operator++(int)
		{
			if(++position == iterating->end())
				ID = -1;
            else
                ID = (*position)->ID;
		}


		bool Graph::VertexIterator::operator==(const Graph::VertexIterator& i) const
		{
			return ID == i.ID;
		}

		bool Graph::VertexIterator::operator!=(const Graph::VertexIterator& i) const
		{
			return ID != i.ID;
		}

		bool Graph::VertexIterator::operator<(const Graph::VertexIterator& i) const
		{
			return ID < i.ID;
		}

		bool Graph::VertexIterator::operator<=(const Graph::VertexIterator& i) const
		{
			return ID <= i.ID;
		}

		bool Graph::VertexIterator::operator>(const Graph::VertexIterator& i) const
		{
			return ID > i.ID;
		}

		bool Graph::VertexIterator::operator>=(const Graph::VertexIterator& i) const
		{
			return ID >= i.ID;
		}

	// @}


	/// \defgroup edgeiteratormethods ''EdgeIterator Methods''
	// @{

        Graph::EdgeIterator::EdgeIterator()
        {
            Owner = NULL;
            iterating = NULL;
            ID = -1;
        }

		Graph::EdgeIterator::EdgeIterator(Graph* owner, int id, list<Graph::Edge*>* Iterating, list<Graph::Edge*>::iterator position)
		{
			Owner = owner;
			Owner->EIterators.insert(this);
			ID = id;
			iterating = Iterating;
			this->position = position;
		}

		Graph::EdgeIterator::EdgeIterator(const Graph::EdgeIterator& i)
		{
			ID = i.ID;
			Owner = i.Owner;
			iterating = i.iterating;
			position = i.position;
			// this is why no default-constructor is used
			if(Owner != NULL)
                Owner->EIterators.insert(this);
		}

		void Graph::EdgeIterator::operator=(const Graph::EdgeIterator& i)
		{
			ID = i.ID;
			iterating = i.iterating;
			position = i.position;
			// this is why no default-constructor is used
			if(Owner != i.Owner)
			{
                if(Owner != NULL)
                    Owner->EIterators.erase(this);
				Owner = i.Owner;
				if(Owner != NULL)
                    Owner->EIterators.insert(this);
			}
		}

		Graph::EdgeIterator::~EdgeIterator()
		{
			Owner->EIterators.erase(this);
		}

		void Graph::EdgeIterator::operator++(int)
		{
			if(++position == iterating->end())
				ID = -1;
            else
                ID = (*position)->ID;
		}

		bool Graph::EdgeIterator::operator==(const Graph::EdgeIterator& i) const
		{
			return ID == i.ID;
		}

		bool Graph::EdgeIterator::operator!=(const Graph::EdgeIterator& i) const
		{
			return ID != i.ID;
		}

		bool Graph::EdgeIterator::operator<(const Graph::EdgeIterator& i) const
		{
			return ID < i.ID;
		}

		bool Graph::EdgeIterator::operator<=(const Graph::EdgeIterator& i) const
		{
			return ID <= i.ID;
		}

		bool Graph::EdgeIterator::operator>(const Graph::EdgeIterator& i) const
		{
			return ID > i.ID;
		}

		bool Graph::EdgeIterator::operator>=(const Graph::EdgeIterator& i) const
		{
			return ID >= i.ID;
		}

	//@}

        void Graph::EdgeIterator::AddUndirectedConnection(Graph::VertexIterator v)
        {
            (*position)->AddUndirectedConnection(Owner->VertexIteratorToPointer(v));
        }

        void Graph::EdgeIterator::AddOutgoingConnection(Graph::VertexIterator v)
        {
            (*position)->AddOutgoingConnection(Owner->VertexIteratorToPointer(v));
        }

        void Graph::EdgeIterator::AddIncomingConnection(Graph::VertexIterator v)
        {
            (*position)->AddIncomingConnection(Owner->VertexIteratorToPointer(v));
        }

        void Graph::EdgeIterator::RemoveUndirectedConnection(Graph::VertexIterator v)
        {
            (*position)->RemoveUndirectedConnection(Owner->VertexIteratorToPointer(v));
        }

        void Graph::EdgeIterator::RemoveOutgoingConnection(Graph::VertexIterator v)
        {
            (*position)->RemoveOutgoingConnection(Owner->VertexIteratorToPointer(v));
        }

        void Graph::EdgeIterator::RemoveIncomingConnection(Graph::VertexIterator v)
        {
            (*position)->RemoveIncomingConnection(Owner->VertexIteratorToPointer(v));
        }

	/// \defgroup iteratorcreation ''Creating Iterators''
	// @{

		/// \defgroup graphsetiterators ''Creating Iterators for the sets of a Graph''
		// @{

			/// \brief <i>begin</i>-Iterator for the set of all vertices in the graph
			Graph::VertexIterator Graph::BeginVertices()
			{
				if(Vertices.empty())
					return EndVertices();
				return VertexIterator(this, Vertices.front()->ID, &Vertices, Vertices.begin());
			}

			/// \brief <i>end</i>-Iterator for the set of all vertices in the graph
			Graph::VertexIterator Graph::EndVertices()
			{
				return VertexIterator(this, -1, &Vertices, Vertices.end());
			}

			/// \brief <i>begin</i>-Iterator for the set of all edges in the graph
			Graph::EdgeIterator Graph::BeginEdges()
			{
				if(Edges.empty())
					return EndEdges();
				return EdgeIterator(this, Edges.front()->ID, &Edges, Edges.begin());
			}

			/// \brief <i>end</i>-Iterator for the set of all edges in the graph
			Graph::EdgeIterator Graph::EndEdges()
			{
				return EdgeIterator(this, -1, &Edges, Edges.end());
			}

		// @}


		/// \defgroup vertexsetiterators ''Creating Iterators for the sets of a Vertex''
		// @{

			Graph::EdgeIterator Graph::VertexIterator::BeginIncidentEdges()
			{
				if((*position)->IncidentEdges.empty())
					return EndIncidentEdges();
				return EdgeIterator((*position)->Owner, (*((*position)->IncidentEdges.begin()))->ID, &(*position)->IncidentEdges, (*position)->IncidentEdges.begin());
			}

			Graph::EdgeIterator Graph::VertexIterator::EndIncidentEdges()
			{
				return EdgeIterator((*position)->Owner, -1, &(*position)->IncidentEdges, (*position)->IncidentEdges.end());
			}

			Graph::EdgeIterator Graph::VertexIterator::BeginPositiveIncidentEdges()
			{
				if((*position)->PositiveIncidentEdges.empty())
					return EndPositiveIncidentEdges();
				return EdgeIterator((*position)->Owner, (*((*position)->PositiveIncidentEdges.begin()))->ID,
                        &(*position)->PositiveIncidentEdges, (*position)->PositiveIncidentEdges.begin());
			}

			Graph::EdgeIterator Graph::VertexIterator::EndPositiveIncidentEdges()
			{
				return EdgeIterator((*position)->Owner, -1, &(*position)->PositiveIncidentEdges, (*position)->PositiveIncidentEdges.end());
			}

			Graph::EdgeIterator Graph::VertexIterator::BeginNegativeIncidentEdges()
			{
				if((*position)->NegativeIncidentEdges.empty())
					return EndNegativeIncidentEdges();
				return EdgeIterator((*position)->Owner, (*((*position)->NegativeIncidentEdges.begin()))->ID,
                        &(*position)->NegativeIncidentEdges, (*position)->NegativeIncidentEdges.begin());
			}

			Graph::EdgeIterator Graph::VertexIterator::EndNegativeIncidentEdges()
			{
				return EdgeIterator((*position)->Owner, -1, &(*position)->NegativeIncidentEdges, (*position)->NegativeIncidentEdges.end());
			}

		// @}


		/// \defgroup edgesetiterators ''Creating Iterators for the sets of an Edge''
		// @{

			/// \brief <i>begin</i>-Iterator for the set of incident vertices of an edge
			Graph::VertexIterator Graph::EdgeIterator::BeginIncidentVertices()
			{
				if((*position)->IncidentVertices.empty())
					return EndIncidentVertices();
				return VertexIterator((*position)->Owner, (*((*position)->IncidentVertices.begin()))->ID, &(*position)->IncidentVertices, (*position)->IncidentVertices.begin());
			}

			/// \brief <i>end</i>-Iterator for the set of incident vertices of an edge
			Graph::VertexIterator Graph::EdgeIterator::EndIncidentVertices()
			{
				return VertexIterator((*position)->Owner, -1, &(*position)->IncidentVertices, (*position)->IncidentVertices.end());
			}

			/// \brief <i>begin</i>-Iterator for the set of positively incident vertices of an edge
			Graph::VertexIterator Graph::EdgeIterator::BeginPositiveIncidentVertices()
			{
				if((*position)->PositiveIncidentVertices.empty())
					return EndPositiveIncidentVertices();
				return VertexIterator((*position)->Owner, (*((*position)->PositiveIncidentVertices.begin()))->ID, &(*position)->PositiveIncidentVertices, (*position)->PositiveIncidentVertices.begin());
			}

			/// \brief <i>end</i>-Iterator for the set of positively incident vertices of an edge
			Graph::VertexIterator Graph::EdgeIterator::EndPositiveIncidentVertices()
			{
				return VertexIterator((*position)->Owner, -1, &(*position)->PositiveIncidentVertices, (*position)->PositiveIncidentVertices.end());
			}

			/// \brief <i>begin</i>-Iterator for the set of negatively incident vertices of an edge
			Graph::VertexIterator Graph::EdgeIterator::BeginNegativeIncidentVertices()
			{
				if((*position)->NegativeIncidentVertices.empty())
					return EndNegativeIncidentVertices();
				return VertexIterator((*position)->Owner, (*((*position)->NegativeIncidentVertices.begin()))->ID, &(*position)->NegativeIncidentVertices, (*position)->NegativeIncidentVertices.begin());
			}

			/// \brief <i>end</i>-Iterator for the set of negatively incident vertices of an edge
			Graph::VertexIterator Graph::EdgeIterator::EndNegativeIncidentVertices()
			{
				return VertexIterator((*position)->Owner, -1, &(*position)->NegativeIncidentVertices, (*position)->NegativeIncidentVertices.end());
			}

			/// \brief Iterator for the first incident vertex of an edge
			Graph::VertexIterator Graph::EdgeIterator::From()
			{
			    Edge* e = *position;
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
			Graph::VertexIterator Graph::EdgeIterator::To()
			{
			    Edge* e = *position;
			    if(e->IncidentVertices.size() == 2
                    && e->NegativeIncidentVertices.size() == 0
                    && e->PositiveIncidentVertices.size() == 0)
                    {
                        Graph::VertexIterator result = BeginIncidentVertices();
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

		/// \defgroup graphaccessors ''Accessor-Methods for Graph-Attributes''
		// @{

			/// \brief Accessor-method for the ID of a graph
			int Graph::GetID() const
			{
				return ID;
			}

			/// \brief Accessor-method for reading the label of a graph
			string Graph::GetLabel() const
			{
				return Label;
			}

			/// \brief Accessor-method for writing the label of a graph
			void Graph::SetLabel(string label)
			{
				Label = label;
			}

		// @}

		/// \defgroup vertexaccessors ''Accessor-Methods for Vertex-Attributes''
		// @{

			/// \brief Accessor-method for reading the ID of a vertex
			int Graph::VertexIterator::GetID() const
			{
				return ID;
			}

			/// \brief Accessor-method for reading the X-coordinate of a vertex
			float Graph::VertexIterator::GetX() const
			{
				return (*position)->X;
			}

			/// \brief Accessor-method for writing the X-coordinate of a vertex
			void Graph::VertexIterator::SetX(float x)
			{
				(*position)->X = x;
			}

			/// \brief Accessor-method for reading the Y-coordinate of a vertex
			float Graph::VertexIterator::GetY() const
			{
				return (*position)->Y;
			}

			/// \brief Accessor-method for writing the Y-coordinate of a vertex
			void Graph::VertexIterator::SetY(float y)
			{
				(*position)->Y = y;
			}

			/// \brief Accessor-method for reading the Label of a vertex
			string Graph::VertexIterator::GetLabel() const
			{
				return (*position)->Label;
			}

			/// \brief Accessor-method for writing the Label of a vertex
			void Graph::VertexIterator::SetLabel(string label)
			{
				(*position)->Label = label;
			}

			/// \brief Accessor-method for reading the Weight of a vertex
			float Graph::VertexIterator::GetWeight() const
			{
				return (*position)->Weight;
			}

			/// \brief Accessor-method for writing the Weight of a vertex
			void Graph::VertexIterator::SetWeight(float weight)
			{
				(*position)->Weight = weight;
			}

			/// \brief Accessor-method for reading the Tag of a vertex
			void* Graph::VertexIterator::GetTag() const
			{
				return (*position)->Tag;
			}

			/// \brief Accessor-method for writing the Tag of a vertex
			void Graph::VertexIterator::SetTag(void* tag)
			{
				(*position)->Tag = tag;
			}

		// @}

		/// \defgroup edgeaccessors ''Accessor-Methods for Edge-Attributes''
		// @{

			/// \brief Accessor-method for reading the ID of an edge
			int Graph::EdgeIterator::GetID() const
			{
				return ID;
			}

			/// \brief Accessor-method for reading the Label of an edge
			string Graph::EdgeIterator::GetLabel() const
			{
				return (*position)->Label;
			}

			/// \brief Accessor-method for writing the Label of an edge
			void Graph::EdgeIterator::SetLabel(string label)
			{
				(*position)->Label = label;
			}

			/// \brief Accessor-method for reading the Weight of an edge
			float Graph::EdgeIterator::GetWeight() const
			{
				return (*position)->Weight;
			}

			/// \brief Accessor-method for writing the Weight of an edge
			void Graph::EdgeIterator::SetWeight(float weight)
			{
				(*position)->Weight = weight;
			}

			/// \brief Accessor-method for reading the Tag of an edge
			void* Graph::EdgeIterator::GetTag() const
			{
				return (*position)->Tag;
			}

			/// \brief Accessor-method for writing the Tag of an edge
			void Graph::EdgeIterator::SetTag(void* tag)
			{
				(*position)->Tag = tag;
			}

		// @}

	// @}


	/// \defgroup streaming
	// @{


		/// \brief Traverse an XML-structure, create a graph from it and copy it to *this
		bool Graph::LoadFromXML(XML* root)
		{
		    list<XML*> gxl = root->FindChildren("gxl");
			if(gxl.size() != 1)
				throw "XML Document must have exactly 1 top element \"gxl\"";

		    map<string, Graph::VertexIterator*> Vertex_XML_ID_to_pointer;
		    bool DefaultDirected = false;

			list<XML*> XMLGraph = gxl.front()->FindChildren("graph");
			if(XMLGraph.size() != 1)
				throw "XML Document must have exactly 1 \"graph\" element below the top element \"gxl\"";

			XML* pGraph = XMLGraph.front();
			Graph G;
			G.SetLabel(pGraph->GetAttribute("Label",""));

			/// load vertices
			list<XML*> nodes = pGraph->FindChildren("node");
			for(list<XML*>::iterator node = nodes.begin(); node != nodes.end(); node++)
			{
				/// create vertex
				Graph::VertexIterator *v = new VertexIterator(G.AddVertex());

				/// assign attributes
				v->SetX((*node)->GetAttributeAsInt("X",-1));
				v->SetY((*node)->GetAttributeAsInt("Y",-1));
				v->SetLabel((*node)->GetAttribute("Label",""));
				v->SetWeight((*node)->GetAttributeAsFloat("Weight",0.0f));

				/// assign XML-ID
				string id = (*node)->GetAttribute("id", "");
				if(id == "")
					return false; // illegal or no ID
				map<string,Graph::VertexIterator*>::iterator j = Vertex_XML_ID_to_pointer.find(id);
				if(j != Vertex_XML_ID_to_pointer.end())
					return false; // same ID twice
				Vertex_XML_ID_to_pointer[id] = v;
			}

			/// load edges
			list<XML*> edges = pGraph->FindChildren("edge");
			for(list<XML*>::iterator edge = edges.begin(); edge != edges.end(); edge++)
			{
				string xmlIsdirected = (*edge)->GetAttribute("isdirected", "");
				bool Directed = DefaultDirected;
				if(xmlIsdirected != "")
                    Directed = (xmlIsdirected == "true");

				string xmlFrom = (*edge)->GetAttribute("from", "");
                map<string,VertexIterator*>::iterator from = Vertex_XML_ID_to_pointer.find(xmlFrom);
				string xmlTo = (*edge)->GetAttribute("to", "");
                map<string,VertexIterator*>::iterator to = Vertex_XML_ID_to_pointer.find(xmlTo);
                if(from == Vertex_XML_ID_to_pointer.end() || to == Vertex_XML_ID_to_pointer.end())
					return false;

				/// create edge
				EdgeIterator e;
				if(Directed)
                    e = G.AddArc(*(from->second), *(to->second));
                else
                    e = G.AddEdge(*(from->second), *(to->second));
				e.SetLabel((*edge)->GetAttribute("Label",""));
				e.SetWeight((*edge)->GetAttributeAsFloat("Weight",0.0f));
			}

			/// load hyperedges
			list<XML*> rels = pGraph->FindChildren("rel");
			for(list<XML*>::iterator rel = rels.begin(); rel != rels.end(); rel++)
			{
			    EdgeIterator e = G.AddLooseEdge();

				/// collect positive incident vertices
				list<XML*> relends = (*rel)->FindChildren("relend");
				for(list<XML*>::iterator relend = relends.begin(); relend != relends.end(); relend++)
				{
					string xmlTarget = (*relend)->GetAttribute("target", ""); // this is NOT necessarily the ID, that it gets internally!
					if(xmlTarget == "") // illegal or no ID
						return false;

					map<string,VertexIterator*>::iterator target = Vertex_XML_ID_to_pointer.find(xmlTarget);
					// no vertex with that ID
					if(target == Vertex_XML_ID_to_pointer.end())
						return false;

                    string direction = (*relend)->GetAttribute("direction", "");
                    if(direction == "none")
                        e.AddUndirectedConnection(*(target->second));
                    else if(direction == "in")
                        e.AddIncomingConnection(*(target->second));
                    else if(direction == "out")
                        e.AddOutgoingConnection(*(target->second));
                    else
                        return false;
				}
			}

			/// G has been successfully loaded, copy it to *this
			operator=(G);
			for(map<string, Graph::VertexIterator*>::iterator cleaner = Vertex_XML_ID_to_pointer.begin(); cleaner != Vertex_XML_ID_to_pointer.end(); cleaner++)
                delete(cleaner->second);
			return true;
		}


		/// \brief Load an XML-structure from an istream using istream& operator>>(istream&, XML&),
		/// then read the graph from the XML structure using bool Graph::LoadFromXML(XML* root)
		bool Graph::LoadFromStream(istream& is)
		{
			XML *xml = new XML;
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

		list<Graph> Graph::LoadGraphsFromXML(XML* root)
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
				list<XML*> XMLGraphs = root->FindChildren("Graph");
				list<Graph> result;
				for(list<XML*>::iterator i = XMLGraphs.begin(); i != XMLGraphs.end(); i++)
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
			XML *xml = new XML;
			is >> *xml;
			list<Graph> result = LoadGraphsFromXML(xml);
			delete xml;
			return result;
		}


		void Graph::WriteToStream(ostream& os, int indent)
		{
		    string* ind = new string[5];
		    ind[0] = string(2*indent, ' ');
		    for(int i = 1; i < 5; i++)
                ind[i] = ind[i-1] + "  ";

			os << ind[0] << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			os << ind[0] << "<!DOCTYPE gxl SYSTEM \"http://www.gupro.de/GXL/gxl-1.0.dtd\">\n";
			os << ind[0] << "<!-- www.Open-Graphtheory.org -->\n";
			os << ind[0] << "<gxl xmlns:xlink=\"http://www.w3.org/1999/xlink\">\n";
			os << ind[1] <<   "<graph id=\"" << GetID() << "\" edgeids=\"true\" edgemode=\"defaultundirected\" hypergraph=\"false\">\n";

			for(VertexIterator i = BeginVertices(); i != EndVertices(); i++)
			{
				os << ind[2] << "<node id=\"v" << i.GetID() << "\">\n";

				os << ind[3] <<   "<attr name=\"name\">\n";
				os << ind[4] <<     "<string>" << i.GetLabel() << "</string>\n";
				os << ind[3] <<   "</attr>\n";

				os << ind[3] <<   "<attr name=\"x\">\n";
				os << ind[4] <<     "<float>" << i.GetX() << "</float>\n";
				os << ind[3] <<   "</attr>\n";

				os << ind[3] <<   "<attr name=\"y\">\n";
				os << ind[4] <<     "<float>" << i.GetY() << "</float>\n";
				os << ind[3] <<   "</attr>\n";

				os << ind[3] <<   "<attr name=\"weight\">\n";
				os << ind[4] <<     "<float>" << i.GetWeight() << "</float>\n";
				os << ind[3] <<   "</attr>\n";

				os << ind[2] << "</node>\n";
			}

			for(EdgeIterator i = BeginEdges(); i != EndEdges(); i++)
			{
				string ClosingTag = "</edge>\n";
				if(!i.IsHyperedge())
				{
                    if(i.IsUndirected())
                        os << ind[2] << "<edge id=\"e" << i.GetID() << "\" from=\"v" << i.From().GetID() << "\" to=\"v" << i.To().GetID() << "\" isdirected=\"false\">\n";
                    else
                        os << ind[2] << "<edge id=\"e" << i.GetID() << "\" from=\"v" << i.From().GetID() << "\" to=\"v" << i.To().GetID() << "\" isdirected=\"true\">\n";
				}
				else
				{
					os << ind[2] << "<rel id=\"e" << i.GetID() << "\">\n";

					for(VertexIterator j = i.BeginIncidentVertices(); j != i.EndIncidentVertices(); j++)
						os << ind[3] << "<relend target=\"v" << j.GetID() << "\" direction=\"none\"/>\n";
					for(VertexIterator j = i.BeginPositiveIncidentVertices(); j != i.EndPositiveIncidentVertices(); j++)
						os << ind[3] << "<relend target=\"v" << j.GetID() << "\" direction=\"out\"/>\n";
					for(VertexIterator j = i.BeginNegativeIncidentVertices(); j != i.EndNegativeIncidentVertices(); j++)
						os << ind[3] << "<relend target=\"v" << j.GetID() << "\" direction=\"in\"/>\n";

					ClosingTag = "</rel>\n";
				}

				os << ind[3] << "<attr name=\"name\">\n";
				os << ind[4] <<   "<string>" << i.GetLabel() << "</string>\n";
				os << ind[3] << "</attr>\n";

				os << ind[3] << "<attr name=\"weight\">\n";
				os << ind[4] <<   "<float>" << i.GetWeight() << "</float>\n";
				os << ind[3] << "</attr>\n";

				os << ind[2] << ClosingTag;
			}

			os << ind[1] <<   "</graph>\n";
			os << ind[0] << "</gxl>\n";
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


		// =========
		// Debugging
		// =========

		void Graph::PrintDebugInfo(ostream& os)
		{
			os << "\nVertices\n--------\n";
			for(list<Vertex*>::iterator it = Vertices.begin(); it != Vertices.end(); it++)
			{
				os << "\n("<<(*it)<<") ID=" << (*it)->ID;
				os.flush();

				os << "\n\tIncident Edges:";
				for(list<Edge*>::iterator jt = (*it)->IncidentEdges.begin(); jt != (*it)->IncidentEdges.end(); jt++)
				{
					os << " " << (*jt);
					os.flush();
				}

				os << "\n\tIncoming Arcs:";
				for(list<Edge*>::iterator jt = (*it)->PositiveIncidentEdges.begin(); jt != (*it)->PositiveIncidentEdges.end(); jt++)
				{
					os << " " << (*jt);
					os.flush();
				}

				os << "\n\tOutgoing Arcs:";
				for(list<Edge*>::iterator jt = (*it)->NegativeIncidentEdges.begin(); jt != (*it)->NegativeIncidentEdges.end(); jt++)
				{
					os << " " << (*jt);
					os.flush();
				}
			}

			os << "\nEdges\n-----\n";
			for(list<Edge*>::iterator it = Edges.begin(); it != Edges.end(); it++)
			{
				os << "(" << *it << ") ID=" << (*it)->ID << "\n\tIncident:";
				os.flush();
				for(list<Vertex*>::iterator jt = (*it)->IncidentVertices.begin(); jt != (*it)->IncidentVertices.end(); jt++)
				{
					os << " " << (*jt);
					os.flush();
				}

				os << "\n\tPositiveIncident:";
				for(list<Vertex*>::iterator jt = (*it)->PositiveIncidentVertices.begin(); jt != (*it)->PositiveIncidentVertices.end(); jt++)
				{
					os << " " << (*jt);
					os.flush();
				}

				os << "\n\tNegativeIncident:";
				for(list<Vertex*>::iterator jt = (*it)->NegativeIncidentVertices.begin(); jt != (*it)->NegativeIncidentVertices.end(); jt++)
				{
					os << " " << (*jt);
					os.flush();
				}
				os << "\n";
			}

			os << "\n-----------------------------------------------------------\n";
            os.flush();
		}

	// @}

} // namespace OpenGraphtheory

