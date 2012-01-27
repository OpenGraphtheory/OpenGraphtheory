
#ifndef __OPENGT_H
	#define __OPENGT_H

	#include <list>
	#include <set>
	#include <map>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include "simple_xml.h"
	using namespace std;

/*
	TODO:
	=====
	need 2-3 videos before launch
	Graph::Clear() doesn't invalidate iterators!

	doxygen compatibility
	induced subgraph from set<VertexIterator>
	components
	methods to get sizes of the sets (EdgesToAdjacentVertices etc)
	graph::label,id -> operator=, LoadFromXML
	melt set<VertexIterator>

	iterators must be instantiated in methods of the iterators, not in methods
		of the actual objects
	operator-> operator* must go (no access to actual objects!)
	loops in exporters
	multiplicity -> distinguish loops from half edges

	changes must invalidate iterators
	thread-safety

	Induced Subgraph
	operators + - * (sets of) vertices, edges, graphs

	meta-infos (tree decomposition or so)
	degree
	Reverse Iterators

	debugging -> print map<int,Vertex*> for ID, XML_Id

	what if loops are added? -> EdgesToAdjacentVertices / ArcsToSuccessors broken?
*/

    namespace OpenGraphtheory
    {

		/// \brief A class to represent graphs
		class Graph
		{
			public:
				class Vertex;
				class Edge;
				class VertexSet;
				class EdgeSet;
				class VertexIterator;
				class EdgeIterator;

			private:
				static int Vertex_IDs;
				static int Edge_IDs;
				map<int,Graph::Vertex*> Vertex_ID_to_pointer;
				map<int,Graph::Edge*> Edge_ID_to_pointer;

				set<Graph::VertexIterator*> VIterators; // invalidate these on changes
				set<Graph::EdgeIterator*> EIterators;

			protected:
				int ID;
				string Label;
				list<Graph::Vertex*> Vertices;
				list<Graph::Edge*> Edges;
				Graph::Vertex* VertexIteratorToPointer(const Graph::VertexIterator& i);
				Graph::Edge* EdgeIteratorToPointer(const Graph::EdgeIterator& i);

			public:
				virtual ~Graph();
				Graph(int size = 0);
				Graph(const Graph& G);
				void Clear();
				void operator=(const Graph& G);
				void PrintDebugInfo(ostream& os);

				int GetID() const;
				string GetLabel() const;
				void SetLabel(string label);
				int NumberOfVertices() const;

				bool IsUndirected();
				bool IsDirected();
				bool IsMixed();
				bool IsHypergraph();
				bool IsHyperDigraph();
				bool IsMixedHypergraph();

				bool HasDirectedLoops();
				bool HasUndirectedLoops();
				bool HasLoops();
				bool IsMultigraph();
				bool IsMultiDigraph();
				bool IsMixedMultigraph();

				bool HasParallelEdges();
				bool IsSimpleGraph();
				bool IsSimpleDigraph();

				Graph::VertexIterator BeginVertices();
				Graph::VertexIterator EndVertices();
				Graph::EdgeIterator BeginEdges();
				Graph::EdgeIterator EndEdges();

			/// adding and removing vertices
			protected:
				Graph::VertexIterator InternalAddVertex(int x = -1, int y = -1, string label = "", float weight = 0, void* tag = NULL, int ID = -1);
				void RemoveVertex(Graph::Vertex* v, bool RemoveIncidentEdges = true);
			public:
				Graph::VertexIterator AddVertex(int x = -1, int y = -1, string label = "", float weight = 0, void* tag = NULL);
				void RemoveVertex(Graph::VertexIterator v, bool RemoveIncidentEdges = true);

			/// adding and removing edges
			protected:
				Graph::EdgeIterator InternalAddEdge(const list<Graph::Vertex*> &own_vertices, const list<Graph::Vertex*> &pFrom, const list<Graph::Vertex*> &pTo, string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				Graph::EdgeIterator InternalAddEdge(const list<Graph::VertexIterator>* vertices = NULL, const list<Graph::VertexIterator>* From = NULL, const list<Graph::VertexIterator>* To = NULL, string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);

				Graph::EdgeIterator InternalAddEdge(const list<Graph::VertexIterator>& vertices, string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				Graph::EdgeIterator InternalAddEdge(Graph::VertexIterator a, Graph::VertexIterator b, string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				Graph::EdgeIterator InternalAddArc(const list<Graph::VertexIterator>& From, const list<Graph::VertexIterator>& To, string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				Graph::EdgeIterator InternalAddArc( Graph::VertexIterator From, Graph::VertexIterator To, string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				void RemoveEdge(Graph::Edge* pe);
			public:
				Graph::EdgeIterator AddEdge(const list<Graph::VertexIterator>& vertices, const list<Graph::VertexIterator>& From, const list<Graph::VertexIterator>& To, string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddEdge(const list<Graph::VertexIterator>& vertices, string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddEdge(Graph::VertexIterator a, Graph::VertexIterator b, string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddArc(const list<Graph::VertexIterator>& From, const list<Graph::VertexIterator>& To, string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddArc( Graph::VertexIterator From, Graph::VertexIterator To, string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddLoop( Graph::VertexIterator v, string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddDirectedLoop( Graph::VertexIterator v, string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddLooseEdge(string label = "", float weight = 1.0, void* tag = NULL);
				void RemoveEdge(Graph::EdgeIterator e);


			/// streaming
			public:
				bool LoadFromXML(XML* root);
				bool LoadFromStream(istream& is);
				friend istream& operator>>(istream& is, Graph& G);
				static list<Graph> LoadGraphsFromXML(XML* root);
				static list<Graph> LoadGraphsFromStream(istream& is);
				void LoadFromFile(string filename);

				void WriteToStream(ostream& os, int indent=0);
				void SaveToFile(string filename);
				friend ostream& operator<<(ostream& os, Graph& G);


            /// \brief A class to represent graphs
            class Vertex
            {
                friend class Graph;
                private:
                    int ID;
                protected:
                    Vertex(Graph* owner, int x = -1, int y = -1, string label = "", float weight = 1, void* tag = NULL);
                    Graph* Owner;

                    list<Graph::Edge*> IncidentEdges;
                    list<Graph::Edge*> NegativeIncidentEdges;
                    list<Graph::Edge*> PositiveIncidentEdges;

                    float X;
                    float Y;
                    string Label;
                    float Weight;
                    void* Tag;

                    bool Adjacent(const Vertex* v) const;
            };

            class Edge
            {
                friend class Graph;
                private:
                    int ID;
                protected:
                    Edge(Graph* owner, string label, float weight, void* tag);
                    Graph* Owner;

                    list<Graph::Vertex*> IncidentVertices;
                    list<Graph::Vertex*> NegativeIncidentVertices;
                    list<Graph::Vertex*> PositiveIncidentVertices;

                    string Label;
                    float Weight;
                    void* Tag;

                    bool Incident(const Graph::Vertex* v) const;
                    void AddUndirectedConnection(Graph::Vertex* v);
                    void AddOutgoingConnection(Graph::Vertex* v);
                    void AddIncomingConnection(Graph::Vertex* v);
                    void RemoveUndirectedConnection(Graph::Vertex* v);
                    void RemoveOutgoingConnection(Graph::Vertex* v);
                    void RemoveIncomingConnection(Graph::Vertex* v);
            };

/*
            class VertexSet
            {
                friend class Graph;
                private:
                    list<Vertex*> Vertices;
                protected:
                    Graph* Owner;
                    VertexSet(Graph* Owner);
                public:
                    VertexIterator Begin();
                    VertexIterator End();
            };

            class EdgeSet
            {
                friend class Graph;
                private:
                    list<Edge*> Edges;
                protected:
                    Graph* Owner;
                    EdgeSet(Graph* Owner);
                public:
                    EdgeIterator Begin();
                    EdgeIterator End();
            };
*/

            class VertexIterator
            {
                friend class Graph;

                protected:
                    int ID;
                    Graph* Owner;
                    list<Graph::Vertex*>* iterating;
                    list<Graph::Vertex*>::iterator position;
                    VertexIterator(Graph* owner, int id, list<Graph::Vertex*>* Iterating, list<Graph::Vertex*>::iterator position);
                    bool IteratorIsValid;

                public:
                    VertexIterator();
                    VertexIterator(const Graph::VertexIterator& i);
                    void operator=(const Graph::VertexIterator& i);
                    ~VertexIterator();
                    void operator++(int);
                    bool operator==(const Graph::VertexIterator& i) const;
                    bool operator!=(const Graph::VertexIterator& i) const;
                    bool operator<(const Graph::VertexIterator& i) const;
                    bool operator<=(const Graph::VertexIterator& i) const;
                    bool operator>(const Graph::VertexIterator& i) const;
                    bool operator>=(const Graph::VertexIterator& i) const;

                    float GetX() const;
                    void SetX(float X);
                    float GetY() const;
                    void SetY(float Y);
                    string GetLabel() const;
                    void SetLabel(string Label);
                    float GetWeight() const;
                    void SetWeight(float Weight);
                    void* GetTag() const;
                    void SetTag(void* Tag);
                    int GetID() const;

                    Graph::EdgeIterator BeginIncidentEdges();
                    Graph::EdgeIterator EndIncidentEdges();
                    Graph::EdgeIterator BeginPositiveIncidentEdges();
                    Graph::EdgeIterator EndPositiveIncidentEdges();
                    Graph::EdgeIterator BeginNegativeIncidentEdges();
                    Graph::EdgeIterator EndNegativeIncidentEdges();

                    bool Adjacent(const Graph::VertexIterator& to) const;
            };

            class EdgeIterator
            {
                friend class Graph;

                protected:
                    int ID;
                    Graph* Owner;
                    list<Graph::Edge*>* iterating;
                    list<Graph::Edge*>::iterator position;
                    EdgeIterator(Graph* owner, int id, list<Graph::Edge*>* Iterating, list<Graph::Edge*>::iterator position);
                    bool IsValid;

                public:
                    EdgeIterator();
                    EdgeIterator(const Graph::EdgeIterator& i);
                    void operator=(const Graph::EdgeIterator& i);
                    ~EdgeIterator();
                    void operator++(int);
                    bool operator==(const Graph::EdgeIterator& i) const;
                    bool operator!=(const Graph::EdgeIterator& i) const;
                    bool operator<(const Graph::EdgeIterator& i) const;
                    bool operator<=(const Graph::EdgeIterator& i) const;
                    bool operator>(const Graph::EdgeIterator& i) const;
                    bool operator>=(const Graph::EdgeIterator& i) const;

                    string GetLabel() const;
                    void SetLabel(string Label);
                    float GetWeight() const;
                    void SetWeight(float Weight);
                    void* GetTag() const;
                    void SetTag(void* Tag);
                    int GetID() const;

                    Graph::VertexIterator From();
                    Graph::VertexIterator To();

                    bool IsDirected();
                    bool IsUndirected();
                    bool IsLoop();
                    bool IsDirectedLoop();
                    bool IsUndirectedLoop();

                    bool IsEdge();
                    bool IsArc();
                    bool IsHyperedge();
                    bool Incident(const VertexIterator& v) const;

                    Graph::VertexIterator BeginIncidentVertices();
                    Graph::VertexIterator EndIncidentVertices();
                    Graph::VertexIterator BeginPositiveIncidentVertices();
                    Graph::VertexIterator EndPositiveIncidentVertices();
                    Graph::VertexIterator BeginNegativeIncidentVertices();
                    Graph::VertexIterator EndNegativeIncidentVertices();

                    void AddUndirectedConnection(Graph::VertexIterator v);
                    void AddOutgoingConnection(Graph::VertexIterator v);
                    void AddIncomingConnection(Graph::VertexIterator v);
                    void RemoveUndirectedConnection(Graph::VertexIterator v);
                    void RemoveOutgoingConnection(Graph::VertexIterator v);
                    void RemoveIncomingConnection(Graph::VertexIterator v);
            };
		};
    } // namespace OpenGraphtheory

#endif

