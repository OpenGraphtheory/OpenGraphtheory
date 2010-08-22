
#ifndef __OPENGREASY_H
	#define __OPENGREASY_H

	#include <vector>
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


	/// \brief A class to represent graphs
	class Graph {
		public:
			class Vertex;
			class Edge;
			class Arc;
			class VertexIterator;
			class EdgeIterator;
			class ArcIterator;

		private:
			static int Vertex_IDs;
			static int Edge_IDs;
			static int Arc_IDs;
			map<int,Graph::Vertex*> Vertex_ID_to_pointer;
			map<int,Graph::Edge*> Edge_ID_to_pointer;
			map<int,Graph::Arc*> Arc_ID_to_pointer;
			map<int,Graph::Vertex*> Vertex_XML_ID_to_pointer;
			map<int,Graph::Edge*> Edge_XML_ID_to_pointer;
			map<int,Graph::Arc*> Arc_XML_ID_to_pointer;

			set<Graph::VertexIterator*> VIterators; // notify these on changes
			set<Graph::EdgeIterator*> EIterators;
			set<Graph::ArcIterator*> AIterators;


		protected:
			int n;
			int ID;
			string Label;
			set<Graph::Vertex*> Vertices;
			set<Graph::Edge*> Edges;
			set<Graph::Arc*> Arcs;
			Graph::Vertex* VertexIteratorToPointer(const Graph::VertexIterator& i);
			Graph::Edge* EdgeIteratorToPointer(const Graph::EdgeIterator& i);
			Graph::Arc* ArcIteratorToPointer(const Graph::ArcIterator& i);
			
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
	
			bool Simple() const;
			bool Multigraph() const;
			bool Multidigraph() const;
			bool Directed() const;
			bool Undirected() const;
			bool Mixed() const;
			bool HasHyperedges();
			bool HasHyperarcs();
			bool HasLoops();
			bool Hypergraph();


		// manipulations (adding / removing vertices, edges, arcs)
		private:
			Graph::VertexIterator InternalAddVertex(int x = -1, int y = -1, string label="", float weight=0, int tag=-1, int XML_Id=-1, int ID=-1);
		public:
			Graph::VertexIterator AddVertex(int x = -1, int y = -1, string label="", float weight=0, int tag=0);
			bool RemoveVertex(Graph::VertexIterator v);

		private:
			Graph::EdgeIterator InternalAddEdge(set<Graph::Vertex*>& own_vertices, string label="", float weight=1.0, int tag=0, int XML_Id=-1, int ID=-1);
			Graph::EdgeIterator InternalAddEdge(set<Graph::VertexIterator>& vertices, string label="", float weight=1.0, int tag=0, int XML_Id=-1, int ID=-1);
			bool RemoveEdge(Graph::Edge* pe);
		public:
			Graph::EdgeIterator AddEdge(set<Graph::VertexIterator>& vertices, string label="", float weight=1.0, int tag=0);
			Graph::EdgeIterator AddEdge(Graph::VertexIterator a, Graph::VertexIterator b, string label="", float weight=1.0, int tag=0);
			bool RemoveEdge(Graph::EdgeIterator e);

		private:
			Graph::ArcIterator InternalAddArc(set<Graph::Vertex*>& From, set<Graph::Vertex*>& To, string label="", float weight=1.0, int tag=0, int XML_Id=-1, int ID=-1);
			Graph::ArcIterator InternalAddArc(set<Graph::VertexIterator>& From, set<Graph::VertexIterator>& To, string label="", float weight=1.0, int tag=0, int XML_Id=-1, int ID=-1);
			bool RemoveArc(Graph::Arc* pa);
		public:
			Graph::ArcIterator AddArc(set<Graph::VertexIterator>& From, set<Graph::VertexIterator>& To, string label="", float weight=1.0, int tag=0);
			Graph::ArcIterator AddArc(Graph::VertexIterator From, Graph::VertexIterator To, string label="", float weight=1.0, int tag=0);
			bool RemoveArc(Graph::ArcIterator a);


		// streaming
		public:
			bool LoadFromXML(XML* root);
			bool LoadFromStream(istream& is);
			friend istream& operator>>(istream& is, Graph& G);
			static vector<Graph> LoadGraphsFromXML(XML* root);
			static vector<Graph> LoadGraphsFromStream(istream& is);
			void LoadFromFile(string filename);

			void WriteToStream(ostream& os, int indent=0);
			void SaveToFile(string filename);
			friend ostream& operator<<(ostream& os, Graph& G);


		// iterating
		public:
			class VertexIterator {
				friend class Graph;
				protected:
					int ID;
					Graph* Owner;
					set<Graph::Vertex*>* iterating;
					set<Graph::Vertex*>::iterator it;
					VertexIterator(Graph* owner, int id, set<Graph::Vertex*>* Iterating, set<Graph::Vertex*>::iterator position);
				public:	
					VertexIterator(const Graph::VertexIterator& i);
					void operator=(const Graph::VertexIterator& i);
					~VertexIterator();
					void operator++(int);
					Graph::Vertex& operator*();
					Graph::Vertex* operator->();
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
					int GetTag() const;
					void SetTag(int Tag);
					int GetID() const;

					Graph::VertexIterator BeginAdjacentVertices();
					Graph::VertexIterator EndAdjacentVertices();
					Graph::EdgeIterator BeginIncidentEdges();
					Graph::EdgeIterator EndIncidentEdges();
					Graph::EdgeIterator BeginEdgesToAdjacentVertex(const Graph::VertexIterator& adjacent);
					Graph::EdgeIterator EndEdgesToAdjacentVertex(const Graph::VertexIterator& adjacent);

					Graph::VertexIterator BeginPredecessors();
					Graph::VertexIterator EndPredecessors();
					Graph::ArcIterator BeginIncomingArcs();
					Graph::ArcIterator EndIncomingArcs();
					Graph::ArcIterator BeginArcsFromPredecessor(const Graph::VertexIterator& predecessor);
					Graph::ArcIterator EndArcsFromPredecessor(const  Graph::VertexIterator& predecessor);

					Graph::VertexIterator BeginSuccessors();
					Graph::VertexIterator EndSuccessors();
					Graph::ArcIterator BeginOutgoingArcs();
					Graph::ArcIterator EndOutgoingArcs();
					Graph::ArcIterator BeginArcsToSuccessor(const Graph::VertexIterator& successor);
					Graph::ArcIterator EndArcsToSuccessor(const Graph::VertexIterator& successor);

					bool Adjacent(const Graph::VertexIterator& to);
					bool HasParallelEdges();
					bool HasParallelArcs();
			};
			
			class EdgeIterator {
				friend class Graph;
				protected:
					int ID;
					Graph* Owner;
					set<Graph::Edge*>* iterating;
					set<Graph::Edge*>::iterator it;
					EdgeIterator(Graph* owner, int id, set<Graph::Edge*>* Iterating, set<Graph::Edge*>::iterator position);
				public:	
					EdgeIterator(const Graph::EdgeIterator& i);
					void operator=(const Graph::EdgeIterator& i);
					~EdgeIterator();
					void operator++(int);
					Graph::Edge& operator*();
					Graph::Edge* operator->();
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
					int GetTag() const;
					void SetTag(int Tag);
					int GetID() const;
					
					Graph::VertexIterator First();
					Graph::VertexIterator Second();
					
					Graph::VertexIterator BeginIncidentVertices();
					Graph::VertexIterator EndIncidentVertices();
					bool IsHyperedge();
					bool IsLoop();
			};

			class ArcIterator {
				friend class Graph;
				protected:
					int ID;
					Graph* Owner;
					set<Graph::Arc*>* iterating;
					set<Graph::Arc*>::iterator it;
					ArcIterator(Graph* owner, int id, set<Graph::Arc*>* Iterating, set<Graph::Arc*>::iterator position);
				public:	
					ArcIterator(const Graph::ArcIterator& i);
					void operator=(const Graph::ArcIterator& i);
					~ArcIterator();
					void operator++(int);
					Graph::Arc& operator*();
					Graph::Arc* operator->();
					bool operator==(const Graph::ArcIterator& i) const;
					bool operator!=(const Graph::ArcIterator& i) const;
					bool operator<(const Graph::ArcIterator& i) const;
					bool operator<=(const Graph::ArcIterator& i) const;
					bool operator>(const Graph::ArcIterator& i) const;
					bool operator>=(const Graph::ArcIterator& i) const;
					
					string GetLabel() const;
					void SetLabel(string Label);
					float GetWeight() const;
					void SetWeight(float Weight);
					int GetTag() const;
					void SetTag(int Tag);
					int GetID() const;

					Graph::VertexIterator From();
					Graph::VertexIterator To();

					Graph::VertexIterator BeginNegativeIncidentVertices();
					Graph::VertexIterator EndNegativeIncidentVertices();
					Graph::VertexIterator BeginPositiveIncidentVertices();
					Graph::VertexIterator EndPositiveIncidentVertices();
					bool IsHyperarc();
			};
			
			Graph::VertexIterator BeginVertices();
			Graph::VertexIterator EndVertices();
			Graph::EdgeIterator BeginEdges();
			Graph::EdgeIterator EndEdges();
			Graph::ArcIterator BeginArcs();
			Graph::ArcIterator EndArcs();
			Graph::VertexIterator GetVertexByXML_ID(int xml_id);
			Graph::EdgeIterator GetEdgeByXML_ID(int xml_id);
			Graph::ArcIterator GetArcByXML_ID(int xml_id);

			class Vertex {
				friend class Graph;
				private:
					int ID;
					int XML_Id;
				protected:
					Vertex(Graph* owner, int x=-1, int y=-1, string label="", float weight=1, int tag=-1);
					Graph* Owner;
					set<Graph::Edge*> IncidentEdges;
					set<Graph::Vertex*> AdjacentVertices;
					map<Graph::Vertex*,set<Graph::Edge*> > EdgesToAdjacentVertices;

					set<Graph::Vertex*> Predecessors;
					set<Graph::Arc*> IncomingArcs;
					map<Graph::Vertex*,set<Graph::Arc*> > ArcsFromPredecessors;

					set<Graph::Vertex*> Successors;
					set<Graph::Arc*> OutgoingArcs;
					map<Graph::Vertex*,set<Graph::Arc*> > ArcsToSuccessors;

					
					float X;
					float Y;
					string Label;
					float Weight;
					int Tag;
			};

			class Edge {
				friend class Graph;
				private:
					int ID;
					int XML_Id;
				protected:
					Edge(Graph* owner, string label, float weight, int tag);
					Graph* Owner;
					set<Graph::Vertex*> IncidentVertices;

					string Label;
					float Weight;
					int Tag;
			};

			class Arc {
				friend class Graph;
				private:
					int ID;
					int XML_Id;
				protected:
					Arc(Graph* owner, string label, float weight, int tag);
					Graph* Owner;
					set<Graph::Vertex*> NegativeIncidentVertices;
					set<Graph::Vertex*> PositiveIncidentVertices;

					string Label;
					float Weight;
					int Tag;
			};
	};

#endif

