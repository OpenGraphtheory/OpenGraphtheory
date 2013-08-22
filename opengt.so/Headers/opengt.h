
#ifndef __OPENGRAPHTHEORY_OPENGT_H
	#define __OPENGRAPHTHEORY_OPENGT_H

	#include <list>
	#include <set>
	#include <map>
	#include <string>
	#include <iostream>
	#include <fstream>
	#include "simple_xml.h"
	#include "attributes.h"

    namespace OpenGraphtheory
    {

		/// \brief A class to represent graphs
		class Graph
		{
			public:
				class Vertex;
				class Edge;
				class VertexIterator;
				class EdgeIterator;

			private:
				static int Vertex_IDs;
				static int Edge_IDs;
				std::map<int,Graph::Vertex*> Vertex_ID_to_pointer;
				std::map<int,Graph::Edge*> Edge_ID_to_pointer;

				std::set<Graph::VertexIterator*> VIterators; // invalidate these on changes
				std::set<Graph::EdgeIterator*> EIterators;

			protected:
				int ID;
				std::string Label;
				std::list<Graph::Vertex*> Vertices;
				std::list<Graph::Edge*> Edges;
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
				std::string GetLabel() const;
				void SetLabel(std::string label);
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
				Graph::EdgeIterator GetEdge(int ID);
				Graph::VertexIterator GetVertex(int ID);

				Graph::VertexIterator Fuse(std::set<Graph::VertexIterator> Vertices);

            /// management of attributes

                void AddVertexSet(std::set<Graph::VertexIterator> V, std::string name);
                void AddEdgeSet(std::set<Graph::EdgeIterator> E, std::string name);
                void AddVertexColoring(std::map<Graph::VertexIterator, int> Coloring, std::string name);
                void AddEdgeColoring(std::map<Graph::EdgeIterator, int> Coloring, std::string name);
                void AddEdgeWeight(std::map<Graph::EdgeIterator, float> Weights, std::string name);

			/// adding and removing vertices
			protected:
				Graph::VertexIterator InternalAddVertex(std::vector<float> coordinates, std::string label = "", float weight = 0, void* tag = NULL, int ID = -1);
				void RemoveVertex(Graph::Vertex* v, bool RemoveIncidentEdges = true);
			public:
				Graph::VertexIterator AddVertex(std::string label="", float weight = 0, void* tag = NULL);
				Graph::VertexIterator AddVertex(float x, std::string label="", float weight = 0, void* tag = NULL);
				Graph::VertexIterator AddVertex(float x, float y, std::string label="", float weight = 0, void* tag = NULL);
				Graph::VertexIterator AddVertex(float x, float y, float z, std::string label = "", float weight = 0, void* tag = NULL);
				void RemoveVertex(Graph::VertexIterator v, bool RemoveIncidentEdges = true);
				Graph operator-(Graph::VertexIterator v);
				void operator-=(Graph::VertexIterator v);

			/// adding and removing edges
			protected:
				Graph::EdgeIterator InternalAddEdge(const std::list<Graph::Vertex*> &own_vertices, const std::list<Graph::Vertex*> &pFrom, const std::list<Graph::Vertex*> &pTo, std::string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				Graph::EdgeIterator InternalAddEdge(const std::list<Graph::VertexIterator>* vertices = NULL, const std::list<Graph::VertexIterator>* From = NULL, const std::list<Graph::VertexIterator>* To = NULL, std::string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);

				Graph::EdgeIterator InternalAddEdge(const std::list<Graph::VertexIterator>& vertices, std::string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				Graph::EdgeIterator InternalAddEdge(Graph::VertexIterator a, Graph::VertexIterator b, std::string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				Graph::EdgeIterator InternalAddArc(const std::list<Graph::VertexIterator>& From, const std::list<Graph::VertexIterator>& To, std::string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				Graph::EdgeIterator InternalAddArc( Graph::VertexIterator From, Graph::VertexIterator To, std::string label = "", float weight = 1.0, void* tag = NULL, int ID = -1);
				void RemoveEdge(Graph::Edge* pe);
			public:
				Graph::EdgeIterator AddEdge(const std::list<Graph::VertexIterator>& vertices, const std::list<Graph::VertexIterator>& From, const std::list<Graph::VertexIterator>& To, std::string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddEdge(const std::list<Graph::VertexIterator>& vertices, std::string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddEdge(Graph::VertexIterator a, Graph::VertexIterator b, std::string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddArc(const std::list<Graph::VertexIterator>& From, const std::list<Graph::VertexIterator>& To, std::string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddArc( Graph::VertexIterator From, Graph::VertexIterator To, std::string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddLoop( Graph::VertexIterator v, std::string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddDirectedLoop( Graph::VertexIterator v, std::string label = "", float weight = 1.0, void* tag = NULL);
				Graph::EdgeIterator AddLooseEdge(std::string label = "", float weight = 1.0, void* tag = NULL);
				void RemoveEdge(Graph::EdgeIterator e);
				Graph operator-(Graph::EdgeIterator v);
				void operator-=(Graph::EdgeIterator v);


			/// streaming
			public:
				bool LoadFromXML(OpenGraphtheory::XML::XML* root);
				bool LoadFromStream(std::istream& is);
				friend std::istream& operator>>(std::istream& is, Graph& G);
				static std::list<Graph> LoadGraphsFromXML(OpenGraphtheory::XML::XML* root);
				static std::list<Graph> LoadGraphsFromStream(std::istream& is);
				void LoadFromFile(std::string filename);

				void WriteToStream(std::ostream& os, int indent=0);
				void SaveToFile(std::string filename);
				friend std::ostream& operator<<(std::ostream& os, Graph& G);


            /// \brief A class to represent vertices
            class Vertex
            {
                friend class Graph;
                private:
                    int ID;
                protected:
                    Vertex(Graph* owner, std::vector<float> coordinates, std::string label = "", float weight = 1, void* tag = NULL);
                    ~Vertex();
                    Graph* Owner;

                    std::list<Graph::Edge*> IncidentEdges;
                    std::list<Graph::Edge*> NegativeIncidentEdges;
                    std::list<Graph::Edge*> PositiveIncidentEdges;

                    AttributeCollection* attributes;

                    std::vector<float> Coordinates;
                    std::string Label;
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
                    Edge(Graph* owner, std::string label, float weight, void* tag);
                    ~Edge();
                    Graph* Owner;

                    std::list<Graph::Vertex*> IncidentVertices;
                    std::list<Graph::Vertex*> NegativeIncidentVertices;
                    std::list<Graph::Vertex*> PositiveIncidentVertices;

                    AttributeCollection* attributes;

                    std::string Label;
                    float Weight;
                    void* Tag;

                    bool Incident(const Graph::Vertex* v, bool Undirected=true, bool Positive=false, bool Negative=false) const;
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
                    std::list<Graph::Vertex*>* iterating;
                    std::list<Graph::Vertex*>::iterator position;
                    VertexIterator(Graph* owner, int id, std::list<Graph::Vertex*>* Iterating, std::list<Graph::Vertex*>::iterator position);
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

                    void WriteToXml(OpenGraphtheory::XML::XML* xml);

                    AttributeCollection& Attributes();
                    std::vector<float> GetCoordinates() const;
                    void SetCoordinates(std::vector<float> coordinates);
                    std::string GetLabel() const;
                    void SetLabel(std::string Label);
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
                    Graph::EdgeIterator GetEdge(const Graph::VertexIterator& to,
                        bool UndirectedToUndirected, bool UndirectedToPositive, bool UndirectedToNegative,
                        bool PositiveToUndirected,   bool PositiveToPositive,   bool PositiveToNegative,
                        bool NegativeToUndirected,   bool NegativeToPositive,   bool NegativeToNegative);
                    bool UnderlyingAdjacent(const Graph::VertexIterator& to) const;
                    std::set<Graph::VertexIterator> CollectNeighbors(
                                    bool UndirectedToUndirected, bool UndirectedToPositive, bool UndirectedToNegative,
                                    bool PositiveToUndirected,   bool PositiveToPositive,   bool PositiveToNegative,
                                    bool NegativeToUndirected,   bool NegativeToPositive,   bool NegativeToNegative);
                    std::set<Graph::EdgeIterator> CollectIncidentEdges(bool Undirected, bool Positive, bool Negative);
                    std::set<Graph::VertexIterator> UnderlyingNeighborhood();
                    std::set<Graph::VertexIterator> Successors();
                    std::set<Graph::VertexIterator> Predecessors();
            };

            class EdgeIterator
            {
                friend class Graph;

                protected:
                    int ID;
                    Graph* Owner;
                    std::list<Graph::Edge*>* iterating;
                    std::list<Graph::Edge*>::iterator position;
                    EdgeIterator(Graph* owner, int id, std::list<Graph::Edge*>* Iterating, std::list<Graph::Edge*>::iterator position);
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

                    void WriteToXml(OpenGraphtheory::XML::XML* xml);

                    AttributeCollection& Attributes();
                    std::string GetLabel() const;
                    void SetLabel(std::string Label);
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
                    bool Incident(const VertexIterator& v, bool Undirected=true, bool Positive=false, bool Negative=false) const;
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

                    std::set<Graph::VertexIterator> CollectIncidentVertices(bool Undirected, bool Positive, bool Negative);
            };
		};



    } // namespace OpenGraphtheory

#endif

