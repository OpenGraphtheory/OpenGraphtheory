
#include "opengt.h"
#include "attributes.h"
#include <stdlib.h>
#include <vector>

namespace OpenGraphtheory
{

	int Graph::Vertex_IDs = 0;
	int Graph::Edge_IDs = 0;


	/// \defgroup constructorsdestructors ''Constructors, Destructors and low-level Methods''
	// @{

		/// \brief Vertex Constructor (protected, accessible for class "Graph")
		Graph::Vertex::Vertex(Graph* owner, vector<float> coordinates, string label, float weight, void* tag)
		{
		    attributes = new AttributeCollection();
			Owner = owner;
			Coordinates = coordinates;
			Label = label;
			Weight = weight;
			Tag = tag;
		}
		Graph::Vertex::~Vertex()
		{
		    delete attributes;
		}

		/// \brief Edge Constructor (protected, accessible for class "Graph")
		Graph::Edge::Edge(Graph* owner, string label, float weight, void* tag)
		{
		    attributes = new AttributeCollection();
			Owner = owner;
			Label = label;
			Weight = weight;
			Tag = tag;
		}
		Graph::Edge::~Edge()
		{
		    delete attributes;
		}

		/// \brief Graph Destructor
		Graph::~Graph()
		{
			Clear();
			delete attributes;
		}

		/// \brief Graph Constructor, creates a graph with "size" vertices
		Graph::Graph(int size)
		{
		    attributes = new AttributeCollection();
			for(int i = 0; i < size; i++)
				AddVertex();
		}

		/// \brief Graph Copy-Constructor
		Graph::Graph(const Graph& G)
		{
		    attributes = new AttributeCollection();
			operator=(G);
		}

        int Graph::NumberOfVertices() const
        {
            return Vertices.size();
        }

        int Graph::NumberOfEdges() const
        {
            return Edges.size();
        }

        /// \brief Operator to copy a graph
        void Graph::operator=(const Graph& G)
        {
		    cout.flush();

            Clear();

            Label = G.GetLabel();
            ID = G.GetID();
            *attributes = *(G.attributes);

            /// copy vertices
            for(list<Graph::Vertex*>::const_iterator v = G.Vertices.begin(); v != G.Vertices.end(); v++)
            {
                Graph::VertexIterator vnew = InternalAddVertex((*v)->Coordinates, (*v)->Label, (*v)->Weight, (*v)->Tag, (*v)->ID);
                vnew.Attributes() = *((*v)->attributes);
            }

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

                Graph::EdgeIterator enew = InternalAddEdge(OwnIncident, OwnPosIncident, OwnNegIncident, (*e)->Label, (*e)->Weight, (*e)->Tag, (*e)->ID);
                enew.Attributes() = *((*e)->attributes);
            }
        }

        /// \brief Remove all vertices and edges
        void Graph::Clear()
        {
            attributes->Clear();
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

            void Graph::VertexIterator::WriteToXml(XML* xml)
            {
                XML* node = new XML("node");
                stringstream s;
                s << "v" << GetID();
                node->AddAttribute("id", s.str());

                // Label
                XML* name = new XML("attr");
                name->AddAttribute("name", "name");
                XML* str = new XML("string");
                str->AddChild(new XML_Text(GetLabel()));
                name->AddChild(str);
                node->AddChild(name);

                // Coordinates
                vector<float> coordinates = GetCoordinates();
                if(coordinates.size() > 0)
                {
                    XML* coords = new XML("attr");
                    coords->AddAttribute("name", "coordinates");
                    XML* vec = new XML("vec");

                    for(unsigned int i = 0; i < coordinates.size(); i++)
                    {
                        XML* flt = new XML("float");
                        stringstream s;
                        s << coordinates[i];
                        flt->AddChild(new XML_Text(s.str()));
                        vec->AddChild(flt);
                    }
                    coords->AddChild(vec);
                    node->AddChild(coords);
                }

                // weight
                XML* weight = new XML("attr");
                weight->AddAttribute("name", "weight");
                XML* flt = new XML("float");
                stringstream t;
                t << GetWeight();
                flt->AddChild(new XML_Text(t.str()));
                weight->AddChild(flt);
                node->AddChild(weight);

                // arbitrary attributes
                Attributes().WriteToXml(node);
                xml->AddChild(node);
            }

            /// \brief Test whether the Vertex is adjacent to the given vertex
            bool Graph::VertexIterator::Adjacent(const Graph::VertexIterator& to) const
            {
                Vertex* v = Owner->VertexIteratorToPointer(to);
                return (*position)->Adjacent(v);
            }

            void Graph::EdgeIterator::WriteToXml(XML* xml)
            {
				string ClosingTag = "</edge>\n";
				XML* edge;
				if(!IsHyperedge())
				{
				    edge = new XML("edge");

				    stringstream id;
                    id << "e" << GetID();
                    edge->AddAttribute("id", id.str());

				    stringstream from;
				    from << "v" << From().GetID();
				    edge->AddAttribute("from", from.str());

				    stringstream to;
				    to << "v" << To().GetID();
				    edge->AddAttribute("to", to.str());

				    edge->AddAttribute("isdirected", IsUndirected() ? "false" : "true");
                }
				else
				{
				    edge = new XML("rel");

                    stringstream id;
                    id << "e" << GetID();
                    edge->AddAttribute("id", id.str());

					for(VertexIterator j = BeginIncidentVertices(); j != EndIncidentVertices(); j++)
					{
					    XML* relend = new XML("relend");
					    stringstream target;
					    target << "v" << j.GetID();
					    relend->AddAttribute("target", target.str());
					    relend->AddAttribute("direction", "none");
					}
					for(VertexIterator j = BeginPositiveIncidentVertices(); j != EndPositiveIncidentVertices(); j++)
					{
					    XML* relend = new XML("relend");
					    stringstream target;
					    target << "v" << j.GetID();
					    relend->AddAttribute("target", target.str());
					    relend->AddAttribute("direction", "out");
					}
					for(VertexIterator j = BeginNegativeIncidentVertices(); j != EndNegativeIncidentVertices(); j++)
					{
					    XML* relend = new XML("relend");
					    stringstream target;
					    target << "v" << j.GetID();
					    relend->AddAttribute("target", target.str());
					    relend->AddAttribute("direction", "in");
					}
				}

                // Label
                XML* name = new XML("attr");
                name->AddAttribute("name", "name");
                XML* str = new XML("string");
                str->AddChild(new XML_Text(GetLabel()));
                name->AddChild(str);
                edge->AddChild(name);

                // weight
                XML* weight = new XML("attr");
                weight->AddAttribute("name", "weight");
                XML* flt = new XML("float");
                stringstream sweight;
                sweight << GetWeight();
                flt->AddChild(new XML_Text(sweight.str()));
                weight->AddChild(flt);
                edge->AddChild(weight);

                // arbitrary attributes
                Attributes().WriteToXml(edge);
                xml->AddChild(edge);
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
		Graph::VertexIterator Graph::InternalAddVertex(std::vector<float> coordinates, string label, float weight, void* tag, int ID)
		{
			Vertex* v = new Vertex(this, coordinates, label, weight, tag);

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
		Graph::VertexIterator Graph::AddVertex(string label, float weight, void* tag)
		{
		    vector<float> coordinates;
			return InternalAddVertex(coordinates,label,weight, tag, -1);
		}

		Graph::VertexIterator Graph::AddVertex(float x, string label, float weight, void* tag)
		{
		    vector<float> coordinates;
		    coordinates.push_back(x);
			return InternalAddVertex(coordinates,label,weight, tag, -1);
		}

		/// \brief Add a new Vertex to the graph
		/// \return VertexIterator that points to the newly created instance
		Graph::VertexIterator Graph::AddVertex(float x, float y, string label, float weight, void* tag)
		{
		    vector<float> coordinates;
		    coordinates.push_back(x);
		    coordinates.push_back(y);
			return InternalAddVertex(coordinates,label,weight, tag, -1);
		}

		/// \brief Add a new Vertex to the graph
		/// \return VertexIterator that points to the newly created instance
		Graph::VertexIterator Graph::AddVertex(float x, float y, float z, string label, float weight, void* tag)
		{
		    vector<float> coordinates;
		    coordinates.push_back(x);
		    coordinates.push_back(y);
		    coordinates.push_back(z);
			return InternalAddVertex(coordinates,label,weight, tag, -1);
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

        void Graph::operator-=(Graph::VertexIterator v)
        {
            RemoveVertex(v);
        }

        Graph Graph::operator-(Graph::VertexIterator v)
        {
            Graph result(*this);
            result.RemoveVertex(v);
            return result;
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

        void Graph::operator-=(Graph::EdgeIterator e)
        {
            RemoveEdge(e);
        }

        Graph Graph::operator-(Graph::EdgeIterator e)
        {
            Graph result(*this);
            result.RemoveEdge(e);
            return result;
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

		Graph::VertexIterator Graph::VertexIterator::operator+(int n) const
		{
		    Graph::VertexIterator result(*this);
		    for(; n > 0; --n)
                result++;
            return result;
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

		Graph::EdgeIterator Graph::EdgeIterator::operator+(int n) const
		{
		    Graph::EdgeIterator result(*this);
		    for(; n > 0; --n)
                result++;
            return result;
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

            AttributeCollection& Graph::Attributes()
            {
                return *attributes;
            }

		// @}

		/// \defgroup vertexaccessors ''Accessor-Methods for Vertex-Attributes''
		// @{

			/// \brief Accessor-method for reading the ID of a vertex
			int Graph::VertexIterator::GetID() const
			{
				return ID;
			}

            std::vector<float> Graph::VertexIterator::GetCoordinates() const
            {
                return (*position)->Coordinates;
            }

            void Graph::VertexIterator::SetCoordinates(std::vector<float> coordinates)
            {
                (*position)->Coordinates = coordinates;
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

            AttributeCollection& Graph::VertexIterator::Attributes()
            {
                return *((*position)->attributes);
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

            AttributeCollection& Graph::EdgeIterator::Attributes()
            {
                return *((*position)->attributes);
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

			list<XML*> XMLGraph = gxl.front()->FindChildren("graph");
			if(XMLGraph.size() != 1)
				throw "XML Document must have exactly 1 \"graph\" element below the top element \"gxl\"";

			XML* pGraph = XMLGraph.front();
			Graph G;

            bool DefaultDirected = true;
            string edgemode = pGraph->GetAttribute("edgemode", "defaultdirected");
            if(edgemode == "defaultdirected" || edgemode == "directed")
                DefaultDirected = true;
            else if(edgemode == "defaultundirected" || edgemode == "undirected")
                DefaultDirected = false;


            /// assign attributes
			list<XML*> attrs = pGraph->FindChildren("attr");
			for(list<XML*>::iterator attr = attrs.begin(); attr != attrs.end(); attr++)
                G.Attributes().Set(*attr);

            if(G.Attributes().HasAttribute("name"))
            {
                StringAttribute *attrName = dynamic_cast<StringAttribute*>(G.Attributes().GetAttribute("name"));
                if(attrName != NULL)
                {
                    G.SetLabel(attrName->Value);
                    G.Attributes().Unset("name");
                }
            }

			/// load vertices
			list<XML*> nodes = pGraph->FindChildren("node");
			for(list<XML*>::iterator node = nodes.begin(); node != nodes.end(); node++)
			{
				/// create vertex
				Graph::VertexIterator *v = new VertexIterator(G.AddVertex());

				/// assign attributes
				list<XML*> attrs = (*node)->FindChildren("attr");
				for(list<XML*>::iterator attr = attrs.begin(); attr != attrs.end(); attr++)
                    v->Attributes().Set(*attr);

                if(v->Attributes().HasAttribute("name"))
                {
                    StringAttribute *attrName = dynamic_cast<StringAttribute*>(v->Attributes().GetAttribute("name"));
                    if(attrName != NULL)
                    {
                        v->SetLabel(attrName->Value);
                        v->Attributes().Unset("name");
                    }
                }
                if(v->Attributes().HasAttribute("weight"))
                {
                    FloatAttribute *attrWeight = dynamic_cast<FloatAttribute*>(v->Attributes().GetAttribute("weight"));
                    if(attrWeight != NULL)
                    {
                        v->SetWeight(attrWeight->Value);
                        v->Attributes().Unset("weight");
                    }
                }
                if(v->Attributes().HasAttribute("coordinates"))
                {
                    SeqAttribute *attrCoordinates = dynamic_cast<SeqAttribute*>(v->Attributes().GetAttribute("coordinates"));
                    if(attrCoordinates != NULL)
                    {
                        vector<float> coordinates;
                        for(list<Attribute*>::iterator i = attrCoordinates->Value.begin(); i != attrCoordinates->Value.end(); i++)
                        {
                            FloatAttribute* attrCoordinate = dynamic_cast<FloatAttribute*>(*i);
                            if(attrCoordinate != NULL)
                                coordinates.push_back(attrCoordinate->Value);
                        }
                        v->SetCoordinates(coordinates);
                        v->Attributes().Unset("coordinates");
                    }
                }

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

				/// assign attributes
				list<XML*> attrs = (*edge)->FindChildren("attr");
				for(list<XML*>::iterator attr = attrs.begin(); attr != attrs.end(); attr++)
                    e.Attributes().Set(*attr);

                if(e.Attributes().HasAttribute("name"))
                {
                    StringAttribute *attrName = dynamic_cast<StringAttribute*>(e.Attributes().GetAttribute("name"));
                    if(attrName != NULL)
                    {
                        e.SetLabel(attrName->Value);
                        e.Attributes().Unset("name");
                    }
                }
                if(e.Attributes().HasAttribute("weight"))
                {
                    FloatAttribute *attrWeight = dynamic_cast<FloatAttribute*>(e.Attributes().GetAttribute("weight"));
                    if(attrWeight != NULL)
                    {
                        e.SetWeight(attrWeight->Value);
                        e.Attributes().Unset("weight");
                    }
                }
            }

			/// load hyperedges
			list<XML*> rels = pGraph->FindChildren("rel");
			for(list<XML*>::iterator rel = rels.begin(); rel != rels.end(); rel++)
			{
			    EdgeIterator e = G.AddLooseEdge();

				/// collect incident vertices
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

                /// assign attributes
				list<XML*> attrs = (*rel)->FindChildren("attr");
				for(list<XML*>::iterator attr = attrs.begin(); attr != attrs.end(); attr++)
                    e.Attributes().Set(*attr);

                if(e.Attributes().HasAttribute("name"))
                {
                    StringAttribute *attrName = dynamic_cast<StringAttribute*>(e.Attributes().GetAttribute("name"));
                    if(attrName != NULL)
                    {
                        e.SetLabel(attrName->Value);
                        e.Attributes().Unset("name");
                    }
                }
                if(e.Attributes().HasAttribute("weight"))
                {
                    FloatAttribute *attrWeight = dynamic_cast<FloatAttribute*>(e.Attributes().GetAttribute("weight"));
                    if(attrWeight != NULL)
                    {
                        e.SetWeight(attrWeight->Value);
                        e.Attributes().Unset("weight");
                    }
                }
			}

			/// G has been successfully loaded, copy it to *this
			*this = G;
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
			os << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
			os << "<!DOCTYPE gxl SYSTEM \"http://www.gupro.de/GXL/gxl-1.0.dtd\">\n";

			XML* gxl = new XML("gxl");
			gxl->AddChild(new XML_Comment("www.Open-Graphtheory.org"));
			gxl->AddAttribute("xmlns:xlink", "http://www.w3.org/1999/xlink");

			XML* graph = new XML("graph");
			stringstream s;
			s << GetID();
			graph->AddAttribute("id", s.str());
			graph->AddAttribute("edgeids", "true");
			graph->AddAttribute("edgemode", "defaultundirected");
			graph->AddAttribute("hypergraph", "true");
            attributes->WriteToXml(graph);

			for(VertexIterator i = BeginVertices(); i != EndVertices(); i++)
                i.WriteToXml(graph);

			for(EdgeIterator i = BeginEdges(); i != EndEdges(); i++)
                i.WriteToXml(graph);

            gxl->AddChild(graph);
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

