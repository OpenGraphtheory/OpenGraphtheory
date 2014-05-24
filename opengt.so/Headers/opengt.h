
#ifndef __OPENGRAPHTHEORY_OPENGT_H
	#define __OPENGRAPHTHEORY_OPENGT_H

	#include <list>
	#include <set>
	#include <map>
	#include <string>
	#include <iostream>
	#include <fstream>
    #include <vector>

	#include "simple_xml.h"
	#include "attributes.h"
	#include "set_helper.h"
	#include "visualize/color.h"

    namespace OpenGraphtheory
    {

		/// \brief A class to represent graphs

		class Vertex;
		typedef SetHelper::VectorSet<Vertex*> VertexSet;
		typedef VertexSet::iterator VertexIterator;
		typedef VertexSet::const_iterator ConstVertexIterator;
        typedef std::map<Vertex*, int> VertexPartitioning;
		typedef std::map<Vertex*, float> VertexWeighting;
		typedef std::map<Vertex*, Visualization::Color> VertexColoring;

		class Edge;
		typedef SetHelper::VectorSet<Edge*> EdgeSet;
		typedef EdgeSet::iterator EdgeIterator;
		typedef EdgeSet::const_iterator ConstEdgeIterator;
		typedef std::map<Edge*, int> EdgePartitioning;
		typedef std::map<Edge*, float> EdgeWeighting;
		typedef std::map<Edge*, Visualization::Color> EdgeColoring;

		class VertexEdgeConnection;
		typedef std::vector<VertexEdgeConnection*> VertexEdgeConnectionSet;
		typedef VertexEdgeConnectionSet::iterator VertexEdgeConnectionIterator;
		typedef VertexEdgeConnectionSet::const_iterator ConstVertexEdgeConnectionIterator;

		typedef std::vector<float> Coordinates;


        class GraphObject
        {
            protected:
                AttributeCollection* attributes;
                int ID;
                GraphObject(int ID);
            public:
                static Factory<GraphObject> GraphObjectFactory;

                GraphObject();
                virtual ~GraphObject();
                void Clear();

                int GetID() const;
                std::string GetIDString() const;
                Attribute* AddAttribute(std::string AttributeName, std::string AttributeType);
                Attribute* GetAttribute(std::string AttributeName);
                void RemoveAttribute(std::string AttributeName);

                std::string GetLabel(std::string Name="name", std::string DefaultValue="");
                void SetLabel(std::string Label, std::string Name="name");
                float GetWeight(std::string Name="weight", float DefaultValue=0.05f);
                void SetWeight(float Weight, std::string Name="weight");
                float GetCapacity(std::string Name="capacity", float DefaultValue=0.0f);
                void SetCapacity(float Capacity, std::string Name="capacity");
                int GetColorClass(std::string Name="color");
                void SetColorClass(int ColorClass, std::string Name="color");
                Coordinates GetCoordinates(std::string Name="coordinates");
                void SetCoordinates(Coordinates& coordinates, std::string Name="coordinates");

                void WriteToXml(OpenGraphtheory::XML::XML* xml, std::string IdPrefix);
                bool LoadFromXml(OpenGraphtheory::XML::XML* xml);
        };

		class Graph : public GraphObject
		{
			private:
				static int Vertex_IDs;
				static int Edge_IDs;
				std::map<int,VertexIterator> *Vertex_ID_to_pointer;
				std::map<int,EdgeIterator> *Edge_ID_to_pointer;
		    protected:
				VertexSet* Vertices;
				EdgeSet* Edges;
                static FactoryRegistrator<GraphObject> GraphRegistrator;

			public:
				virtual ~Graph();
				Graph(int size = 0);
				Graph(const Graph& G);
				#if __cplusplus >= 201103L
                    Graph(Graph&& G); // Move Constructor (C++ 2011 Standard)
                #endif
				void Clear();
				void operator=(const Graph& G);

				int NumberOfVertices() const;
				int NumberOfEdges() const;

				bool IsUndirected();
				bool IsDirected();
				bool IsMixed();
				bool IsUndirectedHypergraph();
				bool IsHyperDigraph();
				bool IsMixedHypergraph();
				bool IsHypergraph();

				bool HasDirectedLoops();
				bool HasUndirectedLoops();
				bool HasLoops();
				bool IsMultigraph();
				bool IsMultiDigraph();
				bool IsMixedMultigraph();

				bool HasParallelEdges();
				bool IsSimpleGraph();
				bool IsSimpleDigraph();

				VertexIterator BeginVertices();
				ConstVertexIterator BeginVertices() const;
				VertexIterator EndVertices();
				ConstVertexIterator EndVertices() const;
				EdgeIterator BeginEdges();
				ConstEdgeIterator BeginEdges() const;
				EdgeIterator EndEdges();
				ConstEdgeIterator EndEdges() const;

				EdgeIterator GetEdge(int ID);
				VertexIterator GetVertex(int ID);

				VertexIterator Fuse(VertexSet& Vertices);

            /// management of attributes

                void AddVertexSet(VertexSet& V, std::string name);
                VertexSet GetVertexSet(std::string name);
                void AddEdgeSet(EdgeSet& E, std::string name);
                EdgeSet GetEdgeSet(std::string name);

                void AddVertexPartitioning(VertexPartitioning& Coloring, std::string name);
                VertexPartitioning GetVertexPartitioning(std::string name);
                void AddEdgePartitioning(EdgePartitioning& Coloring, std::string name);
                EdgePartitioning GetEdgePartitioning(std::string name);

                void AddVertexWeighting(VertexWeighting& Weights, std::string name);
                VertexWeighting GetVertexWeighting(std::string name);
                void AddEdgeWeighting(EdgeWeighting& Weights, std::string name);
                EdgeWeighting GetEdgeWeighting(std::string name);

                void AddVertexColoring(VertexColoring& Coloring, std::string name);
                VertexColoring GetVertexColoring(std::string name);
                void AddEdgeColoring(EdgeColoring& Coloring, std::string name);
                EdgeColoring GetEdgeColoring(std::string name);

			/// adding and removing vertices
			protected:
				VertexIterator InternalAddVertex(int ID = -1);

				VertexIterator InternalAddVertex(std::vector<float> coordinates, std::string label = "", float weight = 0, int ID = -1);
			public:
				VertexIterator AddVertex(std::string label="", float weight = 0);
				VertexIterator AddVertex(float x, std::string label="", float weight = 0);
				VertexIterator AddVertex(float x, float y, std::string label="", float weight = 0);
				VertexIterator AddVertex(float x, float y, float z, std::string label = "", float weight = 0);

				void RemoveVertex(Vertex* v, bool RemoveIncidentEdges = true);
				void RemoveVertex(VertexIterator v, bool RemoveIncidentEdges = true);
				Graph operator-(VertexIterator v);
				void operator-=(VertexIterator v);

			/// adding and removing edges
			protected:
				EdgeIterator InternalAddEdge(Vertex* from, Vertex* to, bool directed=false, int ID = -1);

				EdgeIterator InternalAddEdge(const std::list<Vertex*> &own_vertices, const std::list<Vertex*> &pFrom, const std::list<Vertex*> &pTo, std::string label = "", float weight = 1.0, int ID = -1);
				EdgeIterator InternalAddEdge(const std::list<VertexIterator>* vertices = NULL, const std::list<VertexIterator>* From = NULL, const std::list<VertexIterator>* To = NULL, std::string label = "", float weight = 1.0, int ID = -1);

				EdgeIterator InternalAddEdge(const std::list<VertexIterator>& vertices, std::string label = "", float weight = 1.0, int ID = -1);
				EdgeIterator InternalAddEdge(VertexIterator a, VertexIterator b, std::string label = "", float weight = 1.0, int ID = -1);
				EdgeIterator InternalAddArc(const std::list<VertexIterator>& From, const std::list<VertexIterator>& To, std::string label = "", float weight = 1.0, int ID = -1);
				EdgeIterator InternalAddArc( VertexIterator From, VertexIterator To, std::string label = "", float weight = 1.0, int ID = -1);
			public:
				EdgeIterator AddEdge(VertexIterator a, VertexIterator b);
				EdgeIterator AddArc( VertexIterator From, VertexIterator To);
				EdgeIterator AddUndirectedLoop(VertexIterator v);
				EdgeIterator AddDirectedLoop(VertexIterator v);
				EdgeIterator AddLooseEdge();

				EdgeIterator AddEdge(const std::list<VertexIterator>& vertices, const std::list<VertexIterator>& From, const std::list<VertexIterator>& To, std::string label = "", float weight = 1.0);
				EdgeIterator AddEdge(const std::list<VertexIterator>& vertices, std::string label = "", float weight = 1.0);
				EdgeIterator AddArc(const std::list<VertexIterator>& From, const std::list<VertexIterator>& To, std::string label = "", float weight = 1.0);
				EdgeIterator AddLoop( VertexIterator v, std::string label = "", float weight = 1.0);
				EdgeIterator AddDirectedLoop( VertexIterator v, std::string label = "", float weight = 1.0);
				EdgeIterator AddLooseEdge(std::string label = "", float weight = 1.0);
				void RemoveEdge(Edge* e);
				void RemoveEdge(EdgeIterator e);
				Graph operator-(EdgeIterator v);
				void operator-=(EdgeIterator v);


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
        };


        class VertexEdgeConnection
        {
            friend class Graph;
            friend class Vertex;
            friend class Edge;
            public:
                enum Direction {VertexToEdge=-1, Undirected=0, EdgeToVertex=1};
            protected:
                Vertex* vertex;
                Edge* edge;
                Direction direction;
                std::vector<Coordinates> coordinates;
            public:
                Direction GetDirection() const;
                Vertex* GetVertex();
                Edge* GetEdge();

                void WriteToXml(OpenGraphtheory::XML::XML* xml);
                bool LoadFromXml(OpenGraphtheory::XML::XML* xml);
        };

        /// \brief A class to represent vertices
        class Vertex : public GraphObject
        {
            friend class Graph;
            friend class DefaultInstantiator<GraphObject, Vertex>;
            protected:
                Vertex(Graph* owner = NULL);
                ~Vertex();
                Graph* Owner;

                VertexEdgeConnectionSet Connections;
                static FactoryRegistrator<GraphObject> VertexRegistrator;
            public:
                size_t NumberOfConnections() const;
                VertexEdgeConnectionIterator BeginConnections();
                ConstVertexEdgeConnectionIterator BeginConnections() const;
                VertexEdgeConnectionIterator EndConnections();
                ConstVertexEdgeConnectionIterator EndConnections() const;
                VertexEdgeConnectionIterator AddConnection(Edge* edge, VertexEdgeConnection::Direction direction);
                void RemoveConnection(VertexEdgeConnectionIterator connection);

                Edge* GetEdge(const Vertex* to,
                    bool UndirectedToUndirected = true,  bool UndirectedToPositive = false, bool UndirectedToNegative = false,
                    bool PositiveToUndirected = false,   bool PositiveToPositive = false,   bool PositiveToNegative = false,
                    bool NegativeToUndirected = false,   bool NegativeToPositive = false,   bool NegativeToNegative = false) const;
                bool UnderlyingAdjacent(const Vertex* v) const;
                bool Adjacent(const Vertex* v,
                    bool UndirectedToUndirected = true,  bool UndirectedToPositive = false, bool UndirectedToNegative = false,
                    bool PositiveToUndirected = false,   bool PositiveToPositive = false,   bool PositiveToNegative = false,
                    bool NegativeToUndirected = false,   bool NegativeToPositive = false,   bool NegativeToNegative = false) const;

                EdgeSet CollectIncidentEdges(bool Undirected, bool Positive, bool Negative);
                VertexSet CollectNeighbors(
                    bool UndirectedToUndirected, bool UndirectedToPositive, bool UndirectedToNegative,
                    bool PositiveToUndirected,   bool PositiveToPositive,   bool PositiveToNegative,
                    bool NegativeToUndirected,   bool NegativeToPositive,   bool NegativeToNegative);

                VertexSet UnderlyingNeighborhood();
                VertexSet Successors();
                VertexSet Predecessors();

                void WriteToXml(OpenGraphtheory::XML::XML* xml);
                bool LoadFromXml(OpenGraphtheory::XML::XML* xml);
        };

        /// \brief A class to represent edges
        class Edge : public GraphObject
        {
            friend class Graph;
            friend class Vertex;
            protected:
                Edge(Graph* owner = NULL);
                ~Edge();
                Graph* Owner;

                VertexEdgeConnectionSet Connections;

                void AddUndirectedConnection(VertexIterator v);
                void AddOutgoingConnection(VertexIterator v);
                void AddIncomingConnection(VertexIterator v);
                void RemoveUndirectedConnection(VertexIterator v);
                void RemoveOutgoingConnection(VertexIterator v);
                void RemoveIncomingConnection(VertexIterator v);

                static FactoryRegistrator<GraphObject> EdgeRegistrator;
                static FactoryRegistrator<GraphObject> RelRegistrator;

            public:
                Vertex* From();
                Vertex* To();

                bool Incident(const Vertex* v, bool Undirected=true, bool Positive=false, bool Negative=false) const;
                bool UnderlyingIncident(const Vertex* v) const;
                VertexSet CollectIncidentVertices(bool Undirected, bool Positive, bool Negative);
                bool IsUndirected();
                bool IsEdge();
                bool IsArc();
                bool IsDirected();
                bool IsHyperedge();
                bool IsUndirectedLoop();
                bool IsDirectedLoop();
                bool IsLoop();

                size_t NumberOfConnections() const;
                VertexEdgeConnectionIterator BeginConnections();
                ConstVertexEdgeConnectionIterator BeginConnections() const;
                VertexEdgeConnectionIterator EndConnections();
                ConstVertexEdgeConnectionIterator EndConnections() const;

                VertexEdgeConnectionIterator AddConnection(Vertex* vertex, VertexEdgeConnection::Direction direction);
                void RemoveConnection(VertexEdgeConnectionIterator connection);

                void WriteToXml(OpenGraphtheory::XML::XML* xml);
                bool LoadFromXml(OpenGraphtheory::XML::XML* xml, std::map<std::string, Vertex*>& Vertex_XML_ID_to_pointer, bool DefaultDirected);
        };

    } // namespace OpenGraphtheory

#endif

