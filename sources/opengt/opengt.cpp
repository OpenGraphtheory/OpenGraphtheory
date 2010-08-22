
#ifndef __OPENGT_CPP
	#define __OPENGT_CPP


	#include "opengt.h"
	int Graph::Vertex_IDs = 0;
	int Graph::Edge_IDs = 0;
	int Graph::Arc_IDs = 0;

	// ????
	bool Graph::VertexIterator::Adjacent(const Graph::VertexIterator& to) {
		Vertex* v = Owner->VertexIteratorToPointer(to);
		return (*it)->AdjacentVertices.find(v) != (*it)->AdjacentVertices.end();
	}


	// =========================
	// Constructors, Destructors
	// =========================


	/// \brief Vertex Constructor, protected and accessible to class Graph
	Graph::Vertex::Vertex(Graph* owner, int x, int y, string label, float weight, int tag) {
		Owner = owner;
		X = x;
		Y = y;
		Label = label;
		Weight = weight;
		Tag = tag;
	}

	/// \brief Edge Constructor, protected and accessible to class Graph
	Graph::Edge::Edge(Graph* owner, string label, float weight, int tag) {
		Owner = owner;
		Label = label;
		Weight = weight;
		Tag = tag;
	}

	/// \brief Arc Constructor, protected and accessible to class Graph
	Graph::Arc::Arc(Graph* owner, string label, float weight, int tag) {
		Owner = owner;
		Label = label;
		Weight = weight;
		Tag = tag;
	}

	/// \brief Graph Destructor
	Graph::~Graph() {
		Clear();
	}

	/// \brief Graph Constructor, creates a graph with "size" vertices
	Graph::Graph(int size) {
		n = 0;
		for(int i = 0; i < size; i++)
			AddVertex(-1,-1,"",0, 0);
	}

	/// \brief Graph Copy-Constructor, just calls operator=(const Graph& G)
	Graph::Graph(const Graph& G) {
		operator=(G);
	}



	// =================
	// Low-Level Methods
	// =================

	/// \brief Operator to copy a graph
	void Graph::operator=(const Graph& G) {
		Clear();
		Label = G.GetLabel();
		ID = G.GetID();
		/// copy vertices
		for(set<Vertex*>::iterator it = G.Vertices.begin(); it != G.Vertices.end(); it++)
			InternalAddVertex((*it)->X, (*it)->Y, (*it)->Label, (*it)->Weight, (*it)->Tag, (*it)->XML_Id, (*it)->ID);
		/// copy edges
		for(set<Edge*>::iterator it = G.Edges.begin(); it != G.Edges.end(); it++) {
			set<Vertex*> OwnIncident;
			for(set<Vertex*>::iterator j = (*it)->IncidentVertices.begin(); j != (*it)->IncidentVertices.end(); j++)
				OwnIncident.insert(Vertex_ID_to_pointer[(*j)->ID]);
			InternalAddEdge(OwnIncident, (*it)->Label, (*it)->Weight, (*it)->Tag, (*it)->XML_Id, (*it)->ID);
		}
		/// copy arcs
		for(set<Arc*>::iterator it = G.Arcs.begin(); it != G.Arcs.end(); it++) {
			set<Vertex*> OwnPosIncident;
			for(set<Vertex*>::iterator j = (*it)->PositiveIncidentVertices.begin(); j != (*it)->PositiveIncidentVertices.end(); j++)
				OwnPosIncident.insert(Vertex_ID_to_pointer[(*j)->ID]);
			set<Vertex*> OwnNegIncident;
			for(set<Vertex*>::iterator j = (*it)->NegativeIncidentVertices.begin(); j != (*it)->NegativeIncidentVertices.end(); j++)
				OwnNegIncident.insert(Vertex_ID_to_pointer[(*j)->ID]);
			InternalAddArc(OwnNegIncident, OwnPosIncident, (*it)->Label, (*it)->Weight, (*it)->Tag, (*it)->XML_Id, (*it)->ID);
		}
	}

	/// \brief Remove all vertices, edges and arcs
	void Graph::Clear() {
		/// clear vertices
		for(set<Vertex*>::iterator it = Vertices.begin(); it != Vertices.end(); it++)
			delete *it;
		Vertex_XML_ID_to_pointer.clear();
		Vertex_ID_to_pointer.clear();
		Vertices.clear();
		
		/// clear edges
		for(set<Edge*>::iterator it = Edges.begin(); it != Edges.end(); it++)
			delete *it;
		Edge_XML_ID_to_pointer.clear();
		Edge_ID_to_pointer.clear();
		Edges.clear();

		/// clear arcs
		for(set<Arc*>::iterator it = Arcs.begin(); it != Arcs.end(); it++)
			delete *it;
		Arc_XML_ID_to_pointer.clear();
		Arc_ID_to_pointer.clear();
		Arcs.clear();

		n = 0;
	}

	/// \brief Internal method: extract pointer to actual Vertex object from iterator
	Graph::Vertex* Graph::VertexIteratorToPointer(const Graph::VertexIterator& i) {
		if(i.Owner == this)
			return *(i.it);
		map<int,Vertex*>::iterator j = Vertex_ID_to_pointer.find(i.ID);
		if(j == Vertex_ID_to_pointer.end())
			throw "Tried to access a Vertex that is not in the Graph";
		return j->second;
	}
	
	/// \brief Internal method: extract pointer to actual Edge object from iterator
	Graph::Edge* Graph::EdgeIteratorToPointer(const Graph::EdgeIterator& i) {
		if(i.Owner == this)
			return *(i.it);
		map<int,Edge*>::iterator j = Edge_ID_to_pointer.find(i.ID);
		if(j == Edge_ID_to_pointer.end())
			throw "Tried to access an Edge that is not in the Graph";
		return j->second;
	}

	/// \brief Internal method: extract pointer to actual Arc object from iterator
	Graph::Arc* Graph::ArcIteratorToPointer(const Graph::ArcIterator& i) {
		if(i.Owner == this)
			return *(i.it);
		map<int, Arc*>::iterator j = Arc_ID_to_pointer.find(i.ID);
		if(j == Arc_ID_to_pointer.end())
			throw "Tried to access an Edge that is not in the Graph";
		return j->second;
	}



	// ===========================
	// Basic infos about the graph
	// ===========================

	/// \brief Test whether the EdgeIterator points to a loop
	bool Graph::EdgeIterator::IsLoop() {
		return (*it)->IncidentVertices.size() == 1;
	}
	/// \brief Test whether the graph contains a loop
	bool Graph::HasLoops() {
		for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
			if(e.IsLoop())
				return true;
		return false;
	}
	/// \brief Test whether the VertexIterator points to a vertex that has parallel edges
	bool Graph::VertexIterator::HasParallelEdges() {
		for(map<Graph::Vertex*,set<Graph::Edge*> >::iterator i = (*it)->EdgesToAdjacentVertices.begin(); i != (*it)->EdgesToAdjacentVertices.end(); i++)
			if(i->second.size() > 1)
				return true;
		return false;
	}
	/// \brief Test whether the graph contains parallel edges
	bool Graph::VertexIterator::HasParallelArcs() {
		for(map<Graph::Vertex*,set<Graph::Arc*> >::iterator i = (*it)->ArcsToSuccessors.begin(); i != (*it)->ArcsToSuccessors.end(); i++)
			if(i->second.size() > 1)
				return true;
		return false;
	}

	/// TODO: Test whether the graph
	bool Graph::Simple() const {return false;}
	bool Graph::Multigraph() const {return false;}
	bool Graph::Multidigraph() const {return false;}

	/// \brief Test whether the graph has no undirected edges
	bool Graph::Directed() const {
		return Edges.size() == 0;
	}
	/// \brief Test whether the graph has no directed edges
	bool Graph::Undirected() const {
		return Arcs.size() == 0;
	}
	/// \brief Test whether the graph has directed and undirected edges
	bool Graph::Mixed() const {
		return Edges.size() != 0 && Arcs.size() != 0;
	}

	/// \brief Test whether the EdgeIterator points to an edge with more than 2
	/// incident vertices
	bool Graph::EdgeIterator::IsHyperedge() {
		return (*it)->IncidentVertices.size() > 2;
	}
	/// \brief Test whether the graph contains at least one hyperedge
	bool Graph::HasHyperedges() {
		for(EdgeIterator e = BeginEdges(); e != EndEdges(); e++)
			if(e.IsHyperedge())
				return true;
		return false;
	}
	/// \brief Test whether the ArcIterator points to an arc with more than 2
	/// positive or negative incident vertices
	bool Graph::ArcIterator::IsHyperarc() {
		return (*it)->PositiveIncidentVertices.size() != 1
			|| (*it)->NegativeIncidentVertices.size() != 1;
	}
	/// \brief Test whether the graph contains at least one hyperarc
	bool Graph::HasHyperarcs() {
		for(ArcIterator a = BeginArcs(); a != EndArcs(); a++)
			if(a.IsHyperarc())
				return true;
		return false;
	}
	/// \brief Test whether the graph is a hypergraph
	bool Graph::Hypergraph() {
		return HasHyperedges() || HasHyperarcs();
	}



	// ===================
	// Vertex Manipulation
	// ===================

	/// \brief Internal method: Add a new vertex to the graph, ID can be specified
	/// \return VertexIterator that points to the newly created vertex
	Graph::VertexIterator Graph::InternalAddVertex(int x, int y, string label, float weight, int tag, int XML_Id, int ID) {
		Vertex* v = new Vertex(this, x, y, label, weight, tag);
		n++;
		/// Register ID
		if(ID <= 0) {
			// use a semaphore to make this thread-safe!
			ID = ++Vertex_IDs;
		}
		v->ID = ID;
		Vertex_ID_to_pointer[ID] = v;
		/// Register XML-ID
		v->XML_Id = XML_Id;
		if(XML_Id > 0) {
			Vertex_XML_ID_to_pointer[XML_Id] = v;
		}
		/// Register Vertex in set "Vertices"
		return VertexIterator(this, ID, &Vertices, Vertices.insert(v).first);
	}
	/// \brief Add a new vertex to the graph, ID cannot be specified
	/// \return VertexIterator that points to the newly created instance
	Graph::VertexIterator Graph::AddVertex(int x, int y, string label, float weight, int tag) {
		return InternalAddVertex(x,y,label,weight, tag, -1, -1);
	}

	/// \brief Remove a vertex from the graph
	/// \param v VertexIterator that points to the vertex which should be removed
	bool Graph::RemoveVertex(Graph::VertexIterator v) {
		Vertex* pv = VertexIteratorToPointer(v);
		/// remove incident edges and arcs (this also removes v from the
		/// EdgesToAdjacentVertices, ArcsFromPredecessor, ArcsToSuccessors etc. sets)
		while(pv->IncidentEdges.size() > 0)
			RemoveEdge(*(pv->IncidentEdges.begin()));
		while(pv->OutgoingArcs.size() > 0)
			RemoveArc(*(pv->OutgoingArcs.begin()));
		while(pv->IncomingArcs.size() > 0)
			RemoveArc(*(pv->IncomingArcs.begin()));
		/// unregister from the graph-wide maps
		Vertex_ID_to_pointer.erase(pv->ID);
		Vertex_XML_ID_to_pointer.erase(pv->XML_Id);
		Vertices.erase(pv);
		/// free memory
		delete(pv);
		return true;
	}



	// =================
	// Edge Manipulation
	// =================

	Graph::EdgeIterator Graph::InternalAddEdge(set<Graph::Vertex*>& own_vertices, string label, float weight, int tag, int XML_Id, int ID) {
		Edge* e = new Edge(this, label, weight, tag);
		e->IncidentVertices = own_vertices;
		for(set<Vertex*>::iterator it = e->IncidentVertices.begin(); it != e->IncidentVertices.end(); it++) {
			(*it)->IncidentEdges.insert(e);
			set<Vertex*>::iterator jt = it;
			for(jt++; jt != e->IncidentVertices.end(); jt++) {
				(*jt)->EdgesToAdjacentVertices[*it].insert(e);
				(*it)->EdgesToAdjacentVertices[*jt].insert(e);
				(*it)->AdjacentVertices.insert(*jt);
				(*jt)->AdjacentVertices.insert(*it);
			}
		}
		if(ID <= 0) {
			// use a semaphore to make this thread-safe!
			ID = ++Edge_IDs;
		}
		e->ID = ID;
		Edge_ID_to_pointer[ID] = e;
		e->XML_Id = XML_Id;
		if(XML_Id > 0)
			Edge_XML_ID_to_pointer[XML_Id] = e;
		return EdgeIterator(this, ID, &Edges, Edges.insert(e).first);
	}
	Graph::EdgeIterator Graph::InternalAddEdge(set<Graph::VertexIterator>& vertices, string label, float weight, int tag, int XML_Id, int ID) {
		set<Vertex*> own_vertices;
		for(set<VertexIterator>::iterator i = vertices.begin(); i != vertices.end(); i++)
			own_vertices.insert(VertexIteratorToPointer(*i));
		return InternalAddEdge(own_vertices, label, weight, tag, XML_Id, ID);
	}

	Graph::EdgeIterator Graph::AddEdge(set<Graph::VertexIterator>& vertices, string label, float weight, int tag) {
		return InternalAddEdge(vertices, label, weight, tag, -1, -1);
	}

	Graph::EdgeIterator Graph::AddEdge(Graph::VertexIterator a, Graph::VertexIterator b, string label, float weight, int tag) {
		set<VertexIterator> s;
		s.insert(a);
		s.insert(b);
		return AddEdge(s, label, weight, tag);
	}


	bool Graph::RemoveEdge(Graph::Edge* pe) {
		// adjust the sets in the incident vertices
		for(set<Vertex*>::iterator it = pe->IncidentVertices.begin(); it != pe->IncidentVertices.end(); it++) {
			set<Vertex*>::iterator jt = it;
			for(jt++ ; jt != pe->IncidentVertices.end(); jt++) {
				(*jt)->EdgesToAdjacentVertices[*it].erase(pe);
				(*it)->EdgesToAdjacentVertices[*jt].erase(pe);
				if((*jt)->EdgesToAdjacentVertices[*it].empty()) { // if it was the last edge between it and jt
					(*jt)->EdgesToAdjacentVertices.erase(*it);
					(*it)->EdgesToAdjacentVertices.erase(*jt);
					(*jt)->AdjacentVertices.erase(*it);
					(*it)->AdjacentVertices.erase(*jt);
				}
			}
			(*it)->IncidentEdges.erase(pe);
		}

		// remove from Edge_ID_to_pointer and Edges, free RAM
		Edge_ID_to_pointer.erase(pe->ID);
		Edge_XML_ID_to_pointer.erase(pe->XML_Id);
		Edges.erase(pe);
		delete(pe);
		return true;
	}
	bool Graph::RemoveEdge(Graph::EdgeIterator e) {
		return RemoveEdge(EdgeIteratorToPointer(e));
	}



	// ================
	// Arc Manipulation
	// ================

	Graph::ArcIterator Graph::InternalAddArc(set<Graph::Vertex*>& From, set<Graph::Vertex*>& To, string label, float weight, int tag, int XML_Id, int ID) {
		Arc* a = new Arc(this, label, weight, tag);
		a->NegativeIncidentVertices = From;
		a->PositiveIncidentVertices = To;
		for(set<Vertex*>::iterator it = To.begin(); it != To.end(); it++) {
			(*it)->IncomingArcs.insert(a);
			for(set<Vertex*>::iterator jt = From.begin(); jt != From.end(); jt++) {
				(*jt)->ArcsToSuccessors[*it].insert(a);
				(*it)->ArcsFromPredecessors[*jt].insert(a);
				(*it)->Predecessors.insert(*jt);
				(*jt)->Successors.insert(*it);
			}
		}
		for(set<Vertex*>::iterator jt = From.begin(); jt != From.end(); jt++)
			(*jt)->OutgoingArcs.insert(a);
		if(ID <= 0) {
			// use a semaphore to make this thread-safe!
			ID = ++Arc_IDs;
		}
		a->ID = ID;
		Arc_ID_to_pointer[ID] = a;
		a->XML_Id = XML_Id;
		if(XML_Id > 0)
			Arc_XML_ID_to_pointer[XML_Id] = a;
		return ArcIterator(this, ID, &Arcs, Arcs.insert(a).first);
	}
	Graph::ArcIterator Graph::InternalAddArc(set<Graph::VertexIterator>& From, set<Graph::VertexIterator>& To, string label, float weight, int tag, int XML_Id, int ID) {
		set<Vertex*> pFrom;
		for(set<VertexIterator>::iterator i = From.begin(); i != From.end(); i++)
			pFrom.insert(VertexIteratorToPointer(*i));
		set<Vertex*> pTo;
		for(set<VertexIterator>::iterator i = To.begin(); i != To.end(); i++)
			pTo.insert(VertexIteratorToPointer(*i));
		return InternalAddArc(pFrom, pTo, label, weight, tag, XML_Id, ID);
	}

	Graph::ArcIterator Graph::AddArc(set<Graph::VertexIterator>& From, set<Graph::VertexIterator>& To, string label, float weight, int tag) {
		return InternalAddArc(From, To, label, weight, tag, -1, -1);
	}

	Graph::ArcIterator Graph::AddArc(Graph::VertexIterator From, Graph::VertexIterator To, string label, float weight, int tag) {
		set<VertexIterator> f;
		f.insert(From);
		set<VertexIterator> t;
		t.insert(To);
		return AddArc(f, t, label, weight, tag);
	}


	bool Graph::RemoveArc(Graph::Arc* pa) {
		// adjust the sets in the incident vertices
		for(set<Vertex*>::iterator it = pa->PositiveIncidentVertices.begin(); it != pa->PositiveIncidentVertices.end(); it++) {
			for(set<Vertex*>::iterator jt = pa->NegativeIncidentVertices.begin(); jt != pa->NegativeIncidentVertices.end(); jt++) {
				(*jt)->ArcsToSuccessors[*it].erase(pa);
				(*it)->ArcsFromPredecessors[*jt].erase(pa);
				if((*jt)->ArcsToSuccessors[*it].empty()) { // if it was the last arc from jt to it
					(*jt)->ArcsToSuccessors.erase(*it);
					(*it)->ArcsFromPredecessors.erase(*jt);
					(*jt)->Successors.erase(*it);
					(*it)->Predecessors.erase(*jt);
				}
			}
			(*it)->IncomingArcs.erase(pa);
		}
		for(set<Vertex*>::iterator jt = pa->NegativeIncidentVertices.begin(); jt != pa->NegativeIncidentVertices.end(); jt++)
			(*jt)->OutgoingArcs.erase(pa);

		// remove from Arc_ID_to_pointer and Arcs, free RAM
		Arc_ID_to_pointer.erase(pa->ID);
		Arc_XML_ID_to_pointer.erase(pa->XML_Id);
		Arcs.erase(pa);
		delete(pa);
		return true;
	}
	bool Graph::RemoveArc(Graph::ArcIterator a) {
		return RemoveArc(ArcIteratorToPointer(a));
	}


	// ================
	// Vertex Iterators
	// ================

	Graph::VertexIterator::VertexIterator(Graph* owner, int id, set<Graph::Vertex*>* Iterating, set<Graph::Vertex*>::iterator position) {
		Owner = owner;
		Owner->VIterators.insert(this);
		ID = id;
		iterating = Iterating;
		it = position;
	}

	Graph::VertexIterator::VertexIterator(const Graph::VertexIterator& i) {
		ID = i.ID;
		Owner = i.Owner;
		iterating = i.iterating;
		it = i.it;
		// this is why no default-constructor is used
		Owner->VIterators.insert(this);
	}

	void Graph::VertexIterator::operator=(const Graph::VertexIterator& i) {
		ID = i.ID;
		iterating = i.iterating;
		it = i.it;
		// this is why no default-constructor is used
		if(Owner != i.Owner) {
			Owner->VIterators.erase(this);
			Owner = i.Owner;
			Owner->VIterators.insert(this);
		}
	}

	Graph::VertexIterator::~VertexIterator() {
		Owner->VIterators.erase(this);
	}

	void Graph::VertexIterator::operator++(int) {
		if(++it == iterating->end()) {
			ID = -1;
			return;
		}
		ID = (*it)->ID;
	}

	Graph::Vertex& Graph::VertexIterator::operator*() {
		return **it;
	}
	Graph::Vertex* Graph::VertexIterator::operator->() {
		return *it;
	}
	bool Graph::VertexIterator::operator==(const Graph::VertexIterator& i) const {
		return ID == i.ID;
	}
	bool Graph::VertexIterator::operator!=(const Graph::VertexIterator& i) const {
		return ID != i.ID;
	}
	bool Graph::VertexIterator::operator<(const Graph::VertexIterator& i) const {
		return ID < i.ID;
	}
	bool Graph::VertexIterator::operator<=(const Graph::VertexIterator& i) const {
		return ID <= i.ID;
	}
	bool Graph::VertexIterator::operator>(const Graph::VertexIterator& i) const {
		return ID > i.ID;
	}
	bool Graph::VertexIterator::operator>=(const Graph::VertexIterator& i) const {
		return ID >= i.ID;
	}



	// ==============
	// Edge Iterators
	// ==============

	Graph::EdgeIterator::EdgeIterator(Graph* owner, int id, set<Graph::Edge*>* Iterating, set<Graph::Edge*>::iterator position) {
		Owner = owner;
		Owner->EIterators.insert(this);
		ID = id;
		iterating = Iterating;
		it = position;
	}

	Graph::EdgeIterator::EdgeIterator(const Graph::EdgeIterator& i) {
		ID = i.ID;
		Owner = i.Owner;
		iterating = i.iterating;
		it = i.it;
		// this is why no default-constructor is used
		Owner->EIterators.insert(this);
	}

	void Graph::EdgeIterator::operator=(const Graph::EdgeIterator& i) {
		ID = i.ID;
		iterating = i.iterating;
		it = i.it;
		// this is why no default-constructor is used
		if(Owner != i.Owner) {
			Owner->EIterators.erase(this);
			Owner = i.Owner;
			Owner->EIterators.insert(this);
		}
	}

	Graph::EdgeIterator::~EdgeIterator() {
		Owner->EIterators.erase(this);
	}

	void Graph::EdgeIterator::operator++(int) {
		if(++it == iterating->end()) {
			ID = -1;
			return;
		}
		ID = (*it)->ID;
	}

	Graph::Edge& Graph::EdgeIterator::operator*() {
		return **it;
	}
	Graph::Edge* Graph::EdgeIterator::operator->() {
		return *it;
	}
	bool Graph::EdgeIterator::operator==(const Graph::EdgeIterator& i) const {
		return ID == i.ID;
	}
	bool Graph::EdgeIterator::operator!=(const Graph::EdgeIterator& i) const {
		return ID != i.ID;
	}
	bool Graph::EdgeIterator::operator<(const Graph::EdgeIterator& i) const {
		return ID < i.ID;
	}
	bool Graph::EdgeIterator::operator<=(const Graph::EdgeIterator& i) const {
		return ID <= i.ID;
	}
	bool Graph::EdgeIterator::operator>(const Graph::EdgeIterator& i) const {
		return ID > i.ID;
	}
	bool Graph::EdgeIterator::operator>=(const Graph::EdgeIterator& i) const {
		return ID >= i.ID;
	}



	// =============
	// Arc Iterators
	// =============

	Graph::ArcIterator::ArcIterator(Graph* owner, int id, set<Graph::Arc*>* Iterating, set<Graph::Arc*>::iterator position) {
		Owner = owner;
		Owner->AIterators.insert(this);
		ID = id;
		iterating = Iterating;
		it = position;
	}

	Graph::ArcIterator::ArcIterator(const Graph::ArcIterator& i) {
		ID = i.ID;
		Owner = i.Owner;
		iterating = i.iterating;
		it = i.it;
		// this is why no default-constructor is used
		Owner->AIterators.insert(this);
	}

	void Graph::ArcIterator::operator=(const Graph::ArcIterator& i) {
		ID = i.ID;
		iterating = i.iterating;
		it = i.it;
		// this is why no default-constructor is used
		if(Owner != i.Owner) {
			Owner->AIterators.erase(this);
			Owner = i.Owner;
			Owner->AIterators.insert(this);
		}
	}

	Graph::ArcIterator::~ArcIterator() {
		Owner->AIterators.erase(this);
	}

	void Graph::ArcIterator::operator++(int) {
		if(++it == iterating->end()) {
			ID = -1;
			return;
		}
		ID = (*it)->ID;
	}

	Graph::Arc& Graph::ArcIterator::operator*() {
		return **it;
	}
	Graph::Arc* Graph::ArcIterator::operator->() {
		return *it;
	}
	bool Graph::ArcIterator::operator==(const Graph::ArcIterator& i) const {
		return ID == i.ID;
	}
	bool Graph::ArcIterator::operator!=(const Graph::ArcIterator& i) const {
		return ID != i.ID;
	}
	bool Graph::ArcIterator::operator<(const Graph::ArcIterator& i) const {
		return ID < i.ID;
	}
	bool Graph::ArcIterator::operator<=(const Graph::ArcIterator& i) const {
		return ID <= i.ID;
	}
	bool Graph::ArcIterator::operator>(const Graph::ArcIterator& i) const {
		return ID > i.ID;
	}
	bool Graph::ArcIterator::operator>=(const Graph::ArcIterator& i) const {
		return ID >= i.ID;
	}



	// ==========================================
	// Creating Iterators for the sets of a Graph
	// ==========================================

	Graph::VertexIterator Graph::BeginVertices() {
		if(Vertices.empty())
			return EndVertices();
		return VertexIterator(this, (*Vertices.begin())->ID, &Vertices, Vertices.begin());
	}
	Graph::VertexIterator Graph::EndVertices() {
		return VertexIterator(this, -1, &Vertices, Vertices.end());
	}

	Graph::EdgeIterator Graph::BeginEdges() {
		if(Edges.empty())
			return EndEdges();
		return EdgeIterator(this, (*Edges.begin())->ID, &Edges, Edges.begin());
	}
	Graph::EdgeIterator Graph::EndEdges() {
		return EdgeIterator(this, -1, &Edges, Edges.end());
	}

	Graph::ArcIterator Graph::BeginArcs() {
		if(Arcs.empty())
			return EndArcs();
		return ArcIterator(this, (*Arcs.begin())->ID, &Arcs, Arcs.begin());
	}
	Graph::ArcIterator Graph::EndArcs() {
		return ArcIterator(this, -1, &Arcs, Arcs.end());
	}

	Graph::VertexIterator Graph::GetVertexByXML_ID(int xml_id) {
		map<int,Vertex*>::iterator i = Vertex_XML_ID_to_pointer.find(xml_id);
		if(i == Vertex_XML_ID_to_pointer.end())
			return EndVertices();
		return VertexIterator(this, i->second->ID, &Vertices, Vertices.find(i->second));
	}
	Graph::EdgeIterator Graph::GetEdgeByXML_ID(int xml_id) {
		map<int,Edge*>::iterator i = Edge_XML_ID_to_pointer.find(xml_id);
		if(i == Edge_XML_ID_to_pointer.end())
			return EndEdges();
		return EdgeIterator(this, i->second->ID, &Edges, Edges.find(i->second));
	}
	Graph::ArcIterator Graph::GetArcByXML_ID(int xml_id) {
		map<int,Arc*>::iterator i = Arc_XML_ID_to_pointer.find(xml_id);
		if(i == Arc_XML_ID_to_pointer.end())
			return EndArcs();
		return ArcIterator(this, i->second->ID, &Arcs, Arcs.find(i->second));
	}

	

	// ===========================================
	// Creating Iterators for the sets of a Vertex
	// ===========================================

	Graph::VertexIterator Graph::VertexIterator::BeginAdjacentVertices() {
		if((*it)->AdjacentVertices.empty())
			return EndAdjacentVertices();
		return VertexIterator((*it)->Owner, (*((*it)->AdjacentVertices.begin()))->ID, &(*it)->AdjacentVertices, (*it)->AdjacentVertices.begin());
	}
	Graph::VertexIterator Graph::VertexIterator::EndAdjacentVertices() {
		return VertexIterator((*it)->Owner, -1, &(*it)->AdjacentVertices, (*it)->AdjacentVertices.end());
	}

	Graph::EdgeIterator Graph::VertexIterator::BeginIncidentEdges() {
		if((*it)->IncidentEdges.empty())
			return EndIncidentEdges();
		return EdgeIterator((*it)->Owner, (*((*it)->IncidentEdges.begin()))->ID, &(*it)->IncidentEdges, (*it)->IncidentEdges.begin());
	}
	Graph::EdgeIterator Graph::VertexIterator::EndIncidentEdges() {
		return EdgeIterator((*it)->Owner, -1, &(*it)->IncidentEdges, (*it)->IncidentEdges.end());
	}

	Graph::EdgeIterator Graph::VertexIterator::BeginEdgesToAdjacentVertex(const Graph::VertexIterator& j) {
		Vertex* w = (*it)->Owner->VertexIteratorToPointer(j);
		map<Vertex*, set<Edge*> >::iterator i = (*it)->EdgesToAdjacentVertices.find(w);
		if(i == (*it)->EdgesToAdjacentVertices.end())
			throw "Cannot iterate edges between non-adjacent vertices";
		return EdgeIterator((*it)->Owner, (*(i->second.begin()))->ID, &(i->second), i->second.begin());
	}
	Graph::EdgeIterator Graph::VertexIterator::EndEdgesToAdjacentVertex(const Graph::VertexIterator& j) {
		Vertex* w = (*it)->Owner->VertexIteratorToPointer(j);
		map<Vertex*, set<Edge*> >::iterator i = (*it)->EdgesToAdjacentVertices.find(w);
		if(i == (*it)->EdgesToAdjacentVertices.end())
			throw "Cannot iterate edges between non-adjacent vertices";
		return EdgeIterator((*it)->Owner, -1, &(i->second), i->second.end());
	}


	Graph::VertexIterator Graph::VertexIterator::BeginSuccessors() {
		if((*it)->Successors.empty())
			return EndSuccessors();
		return VertexIterator((*it)->Owner, (*((*it)->Successors.begin()))->ID, &(*it)->Successors, (*it)->Successors.begin());
	}
	Graph::VertexIterator Graph::VertexIterator::EndSuccessors() {
		return VertexIterator((*it)->Owner, -1, &(*it)->Successors, (*it)->Successors.end());
	}

	Graph::ArcIterator Graph::VertexIterator::BeginOutgoingArcs() {
		if((*it)->OutgoingArcs.empty())
			return EndOutgoingArcs();
		return ArcIterator((*it)->Owner, (*((*it)->OutgoingArcs.begin()))->ID, &(*it)->OutgoingArcs, (*it)->OutgoingArcs.begin());
	}
	Graph::ArcIterator Graph::VertexIterator::EndOutgoingArcs() {
		return ArcIterator((*it)->Owner, -1, &(*it)->OutgoingArcs, (*it)->OutgoingArcs.end());
	}

	Graph::ArcIterator Graph::VertexIterator::BeginArcsToSuccessor(const Graph::VertexIterator& j) {
		Vertex* w = (*it)->Owner->VertexIteratorToPointer(j);
		map<Vertex*, set<Arc*> >::iterator i = (*it)->ArcsToSuccessors.find(w);
		if(i == (*it)->ArcsToSuccessors.end())
			throw "Cannot iterate arcs between non-adjacent vertices";
		return ArcIterator((*it)->Owner, (*(i->second.begin()))->ID, &(i->second), i->second.begin());
	}
	Graph::ArcIterator Graph::VertexIterator::EndArcsToSuccessor(const Graph::VertexIterator& j) {
		Vertex* w = (*it)->Owner->VertexIteratorToPointer(j);
		map<Vertex*, set<Arc*> >::iterator i = (*it)->ArcsToSuccessors.find(w);
		if(i == (*it)->ArcsToSuccessors.end())
			throw "Cannot iterate arcs between non-adjacent vertices";
		return ArcIterator((*it)->Owner, -1, &(i->second), i->second.end());
	}

	Graph::VertexIterator Graph::VertexIterator::BeginPredecessors() {
		if((*it)->Predecessors.empty())
			return EndPredecessors();
		return VertexIterator((*it)->Owner, (*((*it)->Predecessors.begin()))->ID, &(*it)->Predecessors, (*it)->Predecessors.begin());
	}
	Graph::VertexIterator Graph::VertexIterator::EndPredecessors() {
		return VertexIterator((*it)->Owner, -1, &(*it)->Predecessors, (*it)->Predecessors.end());
	}

	Graph::ArcIterator Graph::VertexIterator::BeginIncomingArcs() {
		if((*it)->IncomingArcs.empty())
			return EndIncomingArcs();
		return ArcIterator((*it)->Owner, (*((*it)->IncomingArcs.begin()))->ID, &((*it)->IncomingArcs), (*it)->IncomingArcs.begin());
	}
	Graph::ArcIterator Graph::VertexIterator::EndIncomingArcs() {
		return ArcIterator((*it)->Owner, -1, &(*it)->IncomingArcs, (*it)->IncomingArcs.end());
	}

	Graph::ArcIterator Graph::VertexIterator::BeginArcsFromPredecessor(const Graph::VertexIterator& j) {
		Vertex* w = (*it)->Owner->VertexIteratorToPointer(j);
		map<Vertex*, set<Arc*> >::iterator i = (*it)->ArcsFromPredecessors.find(w);
		if(i == (*it)->ArcsFromPredecessors.end())
			throw "Cannot iterate arcs between non-adjacent vertices";
		return ArcIterator((*it)->Owner, (*(i->second.begin()))->ID, &(i->second), i->second.begin());
	}
	Graph::ArcIterator Graph::VertexIterator::EndArcsFromPredecessor(const Graph::VertexIterator& j) {
		Vertex* w = (*it)->Owner->VertexIteratorToPointer(j);
		map<Vertex*, set<Arc*> >::iterator i = (*it)->ArcsFromPredecessors.find(w);
		if(i == (*it)->ArcsFromPredecessors.end())
			throw "Cannot iterate arcs between non-adjacent vertices";
		return ArcIterator((*it)->Owner, -1, &(i->second), i->second.end());
	}



	// ==========================================
	// Creating Iterators for the sets of an Edge
	// ==========================================

	Graph::VertexIterator Graph::EdgeIterator::BeginIncidentVertices() {
		if((*it)->IncidentVertices.empty())
			return EndIncidentVertices();
		return VertexIterator((*it)->Owner, (*((*it)->IncidentVertices.begin()))->ID, &(*it)->IncidentVertices, (*it)->IncidentVertices.begin());
	}
	Graph::VertexIterator Graph::EdgeIterator::EndIncidentVertices() {
		return VertexIterator((*it)->Owner, -1, &(*it)->IncidentVertices, (*it)->IncidentVertices.end());
	}

	Graph::VertexIterator Graph::EdgeIterator::First() {
		return BeginIncidentVertices();
	}
	Graph::VertexIterator Graph::EdgeIterator::Second() {
		Graph::VertexIterator result = First();
		if((*it)->IncidentVertices.size() > 1)
			result++;
		return result;
	}



	// =========================================
	// Creating Iterators for the sets of an Arc
	// =========================================

	Graph::VertexIterator Graph::ArcIterator::BeginPositiveIncidentVertices() {
		if((*it)->PositiveIncidentVertices.empty())
			return EndPositiveIncidentVertices();
		return VertexIterator((*it)->Owner, (*((*it)->PositiveIncidentVertices.begin()))->ID, &(*it)->PositiveIncidentVertices, (*it)->PositiveIncidentVertices.begin());
	}
	Graph::VertexIterator Graph::ArcIterator::EndPositiveIncidentVertices() {
		return VertexIterator((*it)->Owner, -1, &(*it)->PositiveIncidentVertices, (*it)->PositiveIncidentVertices.end());
	}
	Graph::VertexIterator Graph::ArcIterator::BeginNegativeIncidentVertices() {
		if((*it)->NegativeIncidentVertices.empty())
			return EndNegativeIncidentVertices();
		return VertexIterator((*it)->Owner, (*((*it)->NegativeIncidentVertices.begin()))->ID, &(*it)->NegativeIncidentVertices, (*it)->NegativeIncidentVertices.begin());
	}
	Graph::VertexIterator Graph::ArcIterator::EndNegativeIncidentVertices() {
		return VertexIterator((*it)->Owner, -1, &(*it)->NegativeIncidentVertices, (*it)->NegativeIncidentVertices.end());
	}

	Graph::VertexIterator Graph::ArcIterator::From() {
		return BeginNegativeIncidentVertices();
	}
	Graph::VertexIterator Graph::ArcIterator::To() {
		return BeginPositiveIncidentVertices();
	}
	
	

	/// \defgroup accessors ''Accessor-Methods''
	// @{

	/// \defgroup graphaccessors ''Accessor-Methods for Graph-Attributes''
	// @{
	/// \brief Accessor-method for the ID of a graph
	int Graph::GetID() const {
		return ID;
	}
	/// \brief Accessor-method for reading the label of a graph
	string Graph::GetLabel() const {
		return Label;
	}
	/// \brief Accessor-method for writing the label of a graph
	void Graph::SetLabel(string label) {
		Label = label;
	}
	// @}

	/// \defgroup vertexaccessors ''Accessor-Methods for Vertex-Attributes''
	// @{
	/// \brief Accessor-method for reading the ID of a vertex
	int Graph::VertexIterator::GetID() const {
		return ID;
	}
	/// \brief Accessor-method for reading the X-coordinate of a vertex
	float Graph::VertexIterator::GetX() const {
		return (*it)->X;
	}
	/// \brief Accessor-method for writing the X-coordinate of a vertex
	void Graph::VertexIterator::SetX(float x) {
		(*it)->X = x;
	}
	/// \brief Accessor-method for reading the Y-coordinate of a vertex
	float Graph::VertexIterator::GetY() const {
		return (*it)->Y;
	}
	/// \brief Accessor-method for writing the Y-coordinate of a vertex
	void Graph::VertexIterator::SetY(float y) {
		(*it)->Y = y;
	}
	/// \brief Accessor-method for reading the Label of a vertex
	string Graph::VertexIterator::GetLabel() const {
		return (*it)->Label;
	}
	/// \brief Accessor-method for writing the Label of a vertex
	void Graph::VertexIterator::SetLabel(string label) {
		(*it)->Label = label;
	}
	/// \brief Accessor-method for reading the Weight of a vertex
	float Graph::VertexIterator::GetWeight() const {
		return (*it)->Weight;
	}
	/// \brief Accessor-method for writing the Weight of a vertex
	void Graph::VertexIterator::SetWeight(float weight) {
		(*it)->Weight = weight;
	}
	/// \brief Accessor-method for reading the Tag of a vertex
	int Graph::VertexIterator::GetTag() const {
		return (*it)->Tag;
	}
	/// \brief Accessor-method for writing the Tag of a vertex
	void Graph::VertexIterator::SetTag(int tag) {
		(*it)->Tag = tag;
	}
	// @}

	/// \defgroup edgeaccessors ''Accessor-Methods for Edge-Attributes''
	// @{
	/// \brief Accessor-method for reading the ID of an edge
	int Graph::EdgeIterator::GetID() const {
		return ID;
	}
	/// \brief Accessor-method for reading the Label of an edge
	string Graph::EdgeIterator::GetLabel() const {
		return (*it)->Label;
	}
	/// \brief Accessor-method for writing the Label of an edge
	void Graph::EdgeIterator::SetLabel(string label) {
		(*it)->Label = label;
	}
	/// \brief Accessor-method for reading the Weight of an edge
	float Graph::EdgeIterator::GetWeight() const {
		return (*it)->Weight;
	}
	/// \brief Accessor-method for writing the Weight of an edge
	void Graph::EdgeIterator::SetWeight(float weight) {
		(*it)->Weight = weight;
	}
	/// \brief Accessor-method for reading the Tag of an edge
	int Graph::EdgeIterator::GetTag() const {
		return (*it)->Tag;
	}
	/// \brief Accessor-method for writing the Tag of an edge
	void Graph::EdgeIterator::SetTag(int tag) {
		(*it)->Tag = tag;
	}
	// @}

	/// \defgroup arcaccessors ''Accessor-Methods for Arc-Attributes''
	// @{
	/// \brief Accessor-method for reading the ID of an arc
	int Graph::ArcIterator::GetID() const {
		return ID;
	}
	/// \brief Accessor-method for reading the Label of an arc
	string Graph::ArcIterator::GetLabel() const {
		return (*it)->Label;
	}
	/// \brief Accessor-method for writing the Label of an arc
	void Graph::ArcIterator::SetLabel(string label) {
		(*it)->Label = label;
	}
	/// \brief Accessor-method for reading the Weight of an arc
	float Graph::ArcIterator::GetWeight() const {
		return (*it)->Weight;
	}
	/// \brief Accessor-method for writing the Weight of an arc
	void Graph::ArcIterator::SetWeight(float weight) {
		(*it)->Weight = weight;
	}
	/// \brief Accessor-method for reading the Tag of an arc
	int Graph::ArcIterator::GetTag() const {
		return (*it)->Tag;
	}
	/// \brief Accessor-method for writing the Tag of an arc
	void Graph::ArcIterator::SetTag(int tag) {
		(*it)->Tag = tag;
	}
	// @}
	
	// @}


	/// \defgroup streaming
	// @{

		
	/// \brief Traverse an XML-structure, create a graph from it and copy it to *this
	bool Graph::LoadFromXML(XML* root) {
		vector<XML*> XMLGraph = root->FindChildren("Graph");
		if(XMLGraph.size() != 1)
			throw "Trying to load a Graph from illegal XML structure";
		XML* pGraph = XMLGraph.front();
		Graph G;
		G.SetLabel(pGraph->GetAttribute("Label",""));

		/// load vertices
		vector<XML*> V = pGraph->FindChildren("Vertex");
		for(vector<XML*>::iterator i = V.begin(); i != V.end(); i++) {
			
			/// create vertex
			VertexIterator v = G.AddVertex();
			
			/// assign attributes
			v.SetX((*i)->GetAttributeAsInt("X",-1));
			v.SetY((*i)->GetAttributeAsInt("Y",-1));
			v.SetLabel((*i)->GetAttribute("Label",""));
			v.SetWeight((*i)->GetAttributeAsFloat("Weight",0.0f));

			/// assign XML-ID
			int id = (*i)->GetAttributeAsInt("ID", -1); // this is NOT necessarily the ID, that it gets internally!
			if(id <= 0)
				return false; // illegal or no ID
			map<int,Vertex*>::iterator j = G.Vertex_XML_ID_to_pointer.find(id);
			if(j != G.Vertex_XML_ID_to_pointer.end())
				return false; // same ID twice
			G.Vertex_XML_ID_to_pointer[id] = *(v.it);
			v->XML_Id = id;
		}

		/// load edges
		vector<XML*> E = pGraph->FindChildren("Edge");
		for(vector<XML*>::iterator i = E.begin(); i != E.end(); i++) {
		
			/// collect incident vertices
			vector<XML*> IncXML = (*i)->FindChildren("IncidentVertex");
			set<Vertex*> Inc;

			for(vector<XML*>::iterator j = IncXML.begin(); j != IncXML.end(); j++) {
				int id = (*j)->GetAttributeAsInt("ID", -1); // this is NOT necessarily the ID, that it gets internally!
				// illegal or no ID
				if(id <= 0)
					return false;
				map<int,Vertex*>::iterator k = G.Vertex_XML_ID_to_pointer.find(id);
				// no vertex with that ID
				if(k == G.Vertex_XML_ID_to_pointer.end()) 
					return false;
				Inc.insert(k->second);
			}
			
			/// create edge
			EdgeIterator e = G.InternalAddEdge(Inc);
			e.SetLabel((*i)->GetAttribute("Label",""));
			e.SetWeight((*i)->GetAttributeAsFloat("Weight",0.0f));

			/// assign XML-ID
			int eid = (*i)->GetAttributeAsInt("ID", -1); // this is NOT necessarily the ID, that it gets internally!
			// negative or no ID
			if(eid <= 0)
				return false;
			map<int,Edge*>::iterator j = G.Edge_XML_ID_to_pointer.find(eid);
			// same ID twice
			if(j != G.Edge_XML_ID_to_pointer.end())
				return false; 
			G.Edge_XML_ID_to_pointer[eid] = *(e.it);
			e->XML_Id = eid;
		}

		/// load arcs
		vector<XML*> A = pGraph->FindChildren("Arc");
		for(vector<XML*>::iterator i = A.begin(); i != A.end(); i++) {
			
			/// collect positive incident vertices
			vector<XML*> PosIncXML = (*i)->FindChildren("PositiveIncidentVertex");
			set<Vertex*> PosInc;
			for(vector<XML*>::iterator j = PosIncXML.begin(); j != PosIncXML.end(); j++) {
				int id = (*j)->GetAttributeAsInt("ID", -1); // this is NOT necessarily the ID, that it gets internally!
				// illegal or no ID
				if(id <= 0)
					return false;
				map<int,Vertex*>::iterator k = G.Vertex_XML_ID_to_pointer.find(id);
				// no vertex with that ID
				if(k == G.Vertex_XML_ID_to_pointer.end())
					return false;
				PosInc.insert(k->second);
			}
			
			/// collect negative incident vertices
			vector<XML*> NegIncXML = (*i)->FindChildren("NegativeIncidentVertex");
			set<Vertex*> NegInc;
			for(vector<XML*>::iterator j = NegIncXML.begin(); j != NegIncXML.end(); j++) {
				int id = (*j)->GetAttributeAsInt("ID", -1); // this is NOT necessarily the ID, that it gets internally!
				// illegal or no ID
				if(id <= 0)
					return false;
				map<int,Vertex*>::iterator k = G.Vertex_XML_ID_to_pointer.find(id);
				// no vertex with that ID
				if(k == G.Vertex_XML_ID_to_pointer.end())
					return false;
				NegInc.insert(k->second);
			}
			
			/// create arc
			ArcIterator a = G.InternalAddArc(NegInc, PosInc);
			a.SetLabel((*i)->GetAttribute("Label",""));
			a.SetWeight((*i)->GetAttributeAsFloat("Weight",0.0f));

			/// assign XML-ID
			int aid = (*i)->GetAttributeAsInt("ID", -1); // this is NOT necessarily the ID, that it gets internally!
			// negative or no ID
			if(aid <= 0)
				return false;
			map<int,Arc*>::iterator j = G.Arc_XML_ID_to_pointer.find(aid);
			// same ID twice
			if(j != G.Arc_XML_ID_to_pointer.end())
				return false;
			G.Arc_XML_ID_to_pointer[aid] = *(a.it);
			a->XML_Id = aid;
		}

		/// G has been successfully loaded, copy it to *this
		operator=(G);
		return true;
	}


	/// \brief Load an XML-structure from an istream using istream& operator>>(istream&, XML&),
	/// then read the graph from the XML structure using bool Graph::LoadFromXML(XML* root)
	bool Graph::LoadFromStream(istream& is) {
		XML *xml = new XML;
		is >> (*xml);
		bool result = LoadFromXML(xml);
		delete xml;
		return result;
	}

	/// \brief Load the file as an istream and load the graph from it,
	/// using bool Graph::LoadFromStream(istream& is)
	void Graph::LoadFromFile(string filename) {
		ifstream is(filename.c_str());
		LoadFromStream(is);
	}

	istream& operator>>(istream& is, Graph& G) {
		G.LoadFromStream(is);
		return is;
	}

	vector<Graph> Graph::LoadGraphsFromXML(XML* root) {
		if(root->name == "Graph") {
			Graph G;
			if(G.LoadFromXML(root))
				return(vector<Graph>(1,G));
			else
				return(vector<Graph>(0));
		}
		if(root->name == "Graphs") {
			vector<XML*> XMLGraphs = root->FindChildren("Graph");
			vector<Graph> result;
			for(vector<XML*>::iterator i = XMLGraphs.begin(); i != XMLGraphs.end(); i++) {
				Graph G;
				G.LoadFromXML(*i);
				result.push_back(G);
			}
			return result;
		}
		throw "Trying to load a Graph from illegal XML structure";
	}

	vector<Graph> Graph::LoadGraphsFromStream(istream& is) {
		XML *xml = new XML;
		is >> *xml;
		vector<Graph> result = LoadGraphsFromXML(xml);
		delete xml;
		return result;
	}


	void Graph::WriteToStream(ostream& os, int indent) {
		string ind(indent, '\t');
		os << ind << "<Graph>\n";
		for(VertexIterator i = BeginVertices(); i != EndVertices(); i++)
			os << ind << "\t<Vertex X=\"" << i->X << "\" Y=\"" << i->Y << "\" Label=\"" << i->Label << "\" Weight=\"" << i->Weight << "\" ID=\"" << i->ID << "\"/>\n";
		for(EdgeIterator i = BeginEdges(); i != EndEdges(); i++) {
			os << ind << "\t<Edge Label=\"" << i->Label << "\" Weight=\"" << i->Weight << "\" ID=\"" << i->ID << "\">\n";
			for(set<Vertex*>::iterator j = i->IncidentVertices.begin(); j != i->IncidentVertices.end(); j++)
				os << ind << "\t\t<IncidentVertex ID=\"" << (*j)->ID << "\"/>\n";
			os << ind << "\t</Edge>\n";
		}
		for(ArcIterator i = BeginArcs(); i != EndArcs(); i++) {
			os << ind << "\t<Arc Label=\"" << i->Label << "\" " << "Weight=\"" << i->Weight << "\" ID=\"" << i->ID << "\">\n";
			for(set<Vertex*>::iterator j = i->PositiveIncidentVertices.begin(); j != i->PositiveIncidentVertices.end(); j++)
				os << ind << "\t\t<PositiveIncidentVertex ID=\"" << (*j)->ID << "\"/>\n";
			for(set<Vertex*>::iterator j = i->NegativeIncidentVertices.begin(); j != i->NegativeIncidentVertices.end(); j++)
				os << ind << "\t\t<NegativeIncidentVertex ID=\"" << (*j)->ID << "\"/>\n";
			os << ind << "\t</Arc>\n";
		}
		os << ind << "</Graph>\n";
	}

	void Graph::SaveToFile(string filename) {
		ofstream os(filename.c_str());
		WriteToStream(os);
	}
	ostream& operator<<(ostream& os, Graph& G) {
		G.WriteToStream(os);
		return os;
	}

	// @}


	// =========
	// Debugging
	// =========

	void Graph::PrintDebugInfo(ostream& os) {
		os << "\nVertices\n--------\n";
		for(set<Vertex*>::iterator it = Vertices.begin(); it != Vertices.end(); it++) {
			os << "\n("<<(*it)<<") ID=" << (*it)->ID << ", XML-ID=" << (*it)->XML_Id;
			os.flush();
			
			os << "\n\tAdjacent:";
			for(set<Vertex*>::iterator jt = (*it)->AdjacentVertices.begin(); jt != (*it)->AdjacentVertices.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n\tIncident Edges:";
			for(set<Edge*>::iterator jt = (*it)->IncidentEdges.begin(); jt != (*it)->IncidentEdges.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n\tEdges To Adjacent Vertices:";
			for(map<Vertex*, set<Edge*> >::iterator jt = (*it)->EdgesToAdjacentVertices.begin(); jt != (*it)->EdgesToAdjacentVertices.end(); jt++) {
				os << "\n\t\t" << jt->first << ":";
				os.flush();
				for(set<Edge*>::iterator kt = jt->second.begin(); kt != jt->second.end(); kt++) {
					os << " " << (*kt);
					os.flush();
				}
			}

			os << "\n\tSuccessors:";
			for(set<Vertex*>::iterator jt = (*it)->Successors.begin(); jt != (*it)->Successors.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n\tOutgoing Arcs:";
			for(set<Arc*>::iterator jt = (*it)->OutgoingArcs.begin(); jt != (*it)->OutgoingArcs.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n\tArcs To Successors:";
			for(map<Vertex*, set<Arc*> >::iterator jt = (*it)->ArcsToSuccessors.begin(); jt != (*it)->ArcsToSuccessors.end(); jt++) {
				os << "\n\t\t" << jt->first << ":";
				os.flush();
				for(set<Arc*>::iterator kt = jt->second.begin(); kt != jt->second.end(); kt++) {
					os << " " << (*kt);
					os.flush();
				}
			}

			os << "\n\tPredecessors:";
			for(set<Vertex*>::iterator jt = (*it)->Predecessors.begin(); jt != (*it)->Predecessors.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n\tIncoming Arcs:";
			for(set<Arc*>::iterator jt = (*it)->IncomingArcs.begin(); jt != (*it)->IncomingArcs.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n\tArcs To Predecessors:";
			for(map<Vertex*, set<Arc*> >::iterator jt = (*it)->ArcsFromPredecessors.begin(); jt != (*it)->ArcsFromPredecessors.end(); jt++) {
				os << "\n\t\t" << jt->first << ":";
				os.flush();
				for(set<Arc*>::iterator kt = jt->second.begin(); kt != jt->second.end(); kt++) {
					os << " " << (*kt);
					os.flush();
				}
			}
		}
		
		os << "\nEdges\n-----\n";
		for(set<Edge*>::iterator it = Edges.begin(); it != Edges.end(); it++) {
			os << "(" << *it << ") ID=" << (*it)->ID << ", XML-ID=" << (*it)->XML_Id << "\n\tIncident:";
			os.flush();
			for(set<Vertex*>::iterator jt = (*it)->IncidentVertices.begin(); jt != (*it)->IncidentVertices.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n";
		}

		os << "\nArcs\n----\n";
		for(set<Arc*>::iterator it = Arcs.begin(); it != Arcs.end(); it++) {
			os << "(" << *it << ") ID=" << (*it)->ID << ", XML-ID=" << (*it)->XML_Id << "\n\tPositiveIncident:";
			os.flush();
			for(set<Vertex*>::iterator jt = (*it)->PositiveIncidentVertices.begin(); jt != (*it)->PositiveIncidentVertices.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n\tNegativeIncident:";
			os.flush();
			for(set<Vertex*>::iterator jt = (*it)->NegativeIncidentVertices.begin(); jt != (*it)->NegativeIncidentVertices.end(); jt++) {
				os << " " << (*jt);
				os.flush();
			}
			os << "\n";
		}
		os << "\n-----------------------------------------------------------\n";
	}

#endif
