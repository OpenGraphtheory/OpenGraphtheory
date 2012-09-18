
#ifndef __OPENGT_H
	#define __OPENGT_H

	#include <list>
	#include <set>
	#include <map>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include "simple_xml.h"
	#include "attributes.h"
	using namespace std;

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
                AttributeCollection* attributes;

			public:
				virtual ~Graph();
				Graph(int size = 0);
				Graph(const Graph& G);
				void Clear();
				void operator=(const Graph& G);

				int GetID() const;
				string GetLabel() const;
				void SetLabel(string label);
				int NumberOfVertices() const;
				int NumberOfEdges() const;
				AttributeCollection& Attributes();

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

            /// management of attributes

                void AddVertexSet(std::set<Graph::VertexIterator> V, string name);
                void AddEdgeSet(std::set<Graph::EdgeIterator> E, string name);
                void AddVertexColoring(std::map<Graph::VertexIterator, int> Coloring, string name);
                void AddEdgeColoring(std::map<Graph::EdgeIterator, int> Coloring, string name);

			/// adding and removing vertices
			protected:
				Graph::VertexIterator InternalAddVertex(vector<float> coordinates, string label = "", float weight = 0, void* tag = NULL, int ID = -1);
				void RemoveVertex(Graph::Vertex* v, bool RemoveIncidentEdges = true);
			public:
				Graph::VertexIterator AddVertex(string label="", float weight = 0, void* tag = NULL);
				Graph::VertexIterator AddVertex(float x, string label="", float weight = 0, void* tag = NULL);
				Graph::VertexIterator AddVertex(float x, float y, string label="", float weight = 0, void* tag = NULL);
				Graph::VertexIterator AddVertex(float x, float y, float z, string label = "", float weight = 0, void* tag = NULL);
				void RemoveVertex(Graph::VertexIterator v, bool RemoveIncidentEdges = true);
				Graph operator-(Graph::VertexIterator v);
				void operator-=(Graph::VertexIterator v);

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
				Graph operator-(Graph::EdgeIterator v);
				void operator-=(Graph::EdgeIterator v);


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


            /// \brief A class to represent vertices
            class Vertex
            {
                friend class Graph;
                private:
                    int ID;
                protected:
                    Vertex(Graph* owner, vector<float> coordinates, string label = "", float weight = 1, void* tag = NULL);
                    ~Vertex();
                    Graph* Owner;

                    list<Graph::Edge*> IncidentEdges;
                    list<Graph::Edge*> NegativeIncidentEdges;
                    list<Graph::Edge*> PositiveIncidentEdges;

                    AttributeCollection* attributes;

                    vector<float> Coordinates;
                    string Label;
                    float Weight;
                    void* Tag;

                    bool Adjacent(const Vertex* v) const;
                    bool UnderlyingAdjacent(const Vertex* v) const;
            };

            /// \brief A class to represent edges
            class Edge
            {
                friend class Graph;
                private:
                    int ID;
                protected:
                    Edge(Graph* owner, string label, float weight, void* tag);
                    ~Edge();
                    Graph* Owner;

                    list<Graph::Vertex*> IncidentVertices;
                    list<Graph::Vertex*> NegativeIncidentVertices;
                    list<Graph::Vertex*> PositiveIncidentVertices;

                    AttributeCollection* attributes;

                    string Label;
                    float Weight;
                    void* Tag;

                    bool Incident(const Graph::Vertex* v) const;
                    bool UnderlyingIncident(const Graph::Vertex* v) const;
                    void AddUndirectedConnection(Graph::Vertex* v);
                    void AddOutgoingConnection(Graph::Vertex* v);
                    void AddIncomingConnection(Graph::Vertex* v);
                    void RemoveUndirectedConnection(Graph::Vertex* v);
                    void RemoveOutgoingConnection(Graph::Vertex* v);
                    void RemoveIncomingConnection(Graph::Vertex* v);
            };


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
                    VertexIterator operator+(int n) const;
                    bool operator==(const Graph::VertexIterator& i) const;
                    bool operator!=(const Graph::VertexIterator& i) const;
                    bool operator<(const Graph::VertexIterator& i) const;
                    bool operator<=(const Graph::VertexIterator& i) const;
                    bool operator>(const Graph::VertexIterator& i) const;
                    bool operator>=(const Graph::VertexIterator& i) const;

                    void WriteToXml(XML* xml);

                    AttributeCollection& Attributes();
                    vector<float> GetCoordinates() const;
                    void SetCoordinates(vector<float> coordinates);
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
                    bool UnderlyingAdjacent(const Graph::VertexIterator& to) const;
                    set<Graph::VertexIterator> UnderlyingNeighborhood();
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
                    EdgeIterator operator+(int n) const;
                    bool operator==(const Graph::EdgeIterator& i) const;
                    bool operator!=(const Graph::EdgeIterator& i) const;
                    bool operator<(const Graph::EdgeIterator& i) const;
                    bool operator<=(const Graph::EdgeIterator& i) const;
                    bool operator>(const Graph::EdgeIterator& i) const;
                    bool operator>=(const Graph::EdgeIterator& i) const;

                    void WriteToXml(XML* xml);

                    AttributeCollection& Attributes();
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
                    bool UnderlyingIncident(const VertexIterator& v) const;

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
