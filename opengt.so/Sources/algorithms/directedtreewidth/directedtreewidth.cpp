
#include "../../../Headers/algorithms/directedtreewidth/directedtreewidth.h"


// see page 8 and 9 of
// http://www.math2.rwth-aachen.de/~koster/paper/boko09a.pdf


using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

namespace OpenGraphtheory
{
    namespace Algorithms
    {
        namespace DirectedTreewidth
        {

            MultiFactoryRegistrator<Algorithm> AlgorithmDIRECTEDTREEDECOMPOSITION::AlgorithmDirectedTreeDecompositionRegistrator(
                &Algorithm::AlgorithmFactory, "directedtreedecomposition", new DefaultInstantiator<Algorithm, AlgorithmDIRECTEDTREEDECOMPOSITION>(
                "directedtreedecomposition", "returns a directed tree-decomposition of the graph", "http://dx.doi.org/10.1006/jctb.2000.2031"));



            DirectedTreeDecomposition::DirectedTreeDecomposition()
            {
            }

            DirectedTreeDecomposition::~DirectedTreeDecomposition()
            {
                for(std::vector<DirectedTreeDecomposition*>::iterator i = Children.begin(); i != Children.end(); i++)
                    delete *i;
            }



            bool AlgorithmDIRECTEDTREEDECOMPOSITION::FindWeaklyBalancedWSeparation(Graph& G, VertexSet& V, VertexSet& W, VertexIterator WIt,
                                                                                   VertexSet& X, VertexSet& S, VertexSet& Y, size_t k)
            {
cerr << "FindWeaklyBalancedWSeparation   W = {"; cerr.flush();
for(VertexIterator v = W.begin(); v != W.end(); v++)
    cerr << (*v)->GetLabel() << " ";
cerr << "}   ";

cerr << "X = {"; cerr.flush();
for(VertexIterator v = X.begin(); v != X.end(); v++)
    cerr << (*v)->GetLabel() << " ";
cerr << "}   ";

cerr << "S = {"; cerr.flush();
for(VertexIterator v = S.begin(); v != S.end(); v++)
    cerr << (*v)->GetLabel() << " ";
cerr << "}   ";

cerr << "Y = {"; cerr.flush();
for(VertexIterator v = Y.begin(); v != Y.end(); v++)
    cerr << (*v)->GetLabel() << " ";
cerr << "}\n";


                if(S.size() > k)
                    return false;
                if(X.size()*4 > W.size()*3)
                    return false;
                if(Y.size()*4 > W.size()*3)
                    return false;

                if(WIt == W.end()) // all vertices are distributed
                {
                    VertexSet Separator;
                    AllowedVerticesFilter VMinusS;
                    for(VertexIterator v = V.begin(); v != V.end(); v++)
                        if(S.find(*v) == S.end())
                            VMinusS.AllowVertex(*v);
                    AlgorithmVERTEXSEPARATOR separatoralgorithm;
                    if(!separatoralgorithm.FindMinimumVertexSeparator(G, X, Y, Separator, &VMinusS))
                        return false;

                    if(S.size() + Separator.size() > k)
                        return false;

                    SetHelper::DestructiveUnion(S,Separator);
                    return true;
                }

                VertexIterator WItNext = WIt;
                WItNext++;
                Vertex* pWIt = *WIt;

                X.insert(pWIt);                                             // Put WIt into X
                if(FindWeaklyBalancedWSeparation(G,V,W,WItNext, X,S,Y, k))
                    return true;
                X.erase(pWIt);

                Y.insert(pWIt);                                             // Put WIt into Y
                if(FindWeaklyBalancedWSeparation(G,V,W,WItNext, X,S,Y, k))
                    return true;
                Y.erase(pWIt);

                S.insert(pWIt);                                             // Put WIt into S
                if(FindWeaklyBalancedWSeparation(G,V,W,WItNext, X,S,Y, k))
                    return true;
                S.erase(pWIt);

                return false;
            }



            DirectedTreeDecomposition* AlgorithmDIRECTEDTREEDECOMPOSITION::FindDirectedTreeDecomposition(Graph& G, VertexSet& V, VertexSet& W, size_t k)
            {
cerr << "FindDirectedTreeDecomposition   k = " << k << "   V = {"; cerr.flush();
for(VertexIterator v = V.begin(); v != V.end(); v++)
    cerr << (*v)->GetLabel() << " ";
cerr << "}   ";
cerr << "W = {"; cerr.flush();
for(VertexIterator v = W.begin(); v != W.end(); v++)
    cerr << (*v)->GetLabel() << " ";
cerr << "}\n";

                DirectedTreeDecomposition* result = NULL;

                // (the remnant of) V is small enough to return a single bag containing all of V
                if(V.size() <= W.size())
                {
                    result = new DirectedTreeDecomposition;
                    result->Bag = V;
                    return result;
                }

                // find weakly balanced W separation (X,S,Y)
                VertexSet X, S, Y;
                if(!FindWeaklyBalancedWSeparation(G, V, W, W.begin(), X,S,Y,  k)) // treewidth is larger than k
                    return NULL;

                // not sure why... ask Leo Bodlaender... see line 8 on page 9 of
                // http://www.math2.rwth-aachen.de/~koster/paper/boko09a.pdf
                if((X.empty() || Y.empty()) && SetHelper::IsSubset(S,W))
                    for(VertexIterator v = V.begin(); v != V.end(); v++)
                        if(!W.contains(*v))
                        {
                            S.insert(*v);
                            break;
                        }


                // determine the strong components of G-S
                AlgorithmSTRONGCOMPONENTS strongcomponentalgorithm;
                AllowedVerticesFilter VMinusS;
                for(VertexIterator v = V.begin(); v != V.end(); v++)
                    if(S.find(*v) == S.end())
                        VMinusS.AllowVertex(*v);
                map<Vertex*, int> ComponentOfVertex;
                vector<vector<Vertex*> > VerticesInComponent;
                strongcomponentalgorithm.FindStrongComponents(G, ComponentOfVertex, VerticesInComponent, &VMinusS);

                // Recurse on  V' = S union C,  W' = S union (W cap C)   for the strong components C of G-S
                result = new DirectedTreeDecomposition;
                for(vector<vector<Vertex*> >::iterator C = VerticesInComponent.begin(); C != VerticesInComponent.end(); C++)
                {
                    VertexSet CSet;
                    for(vector<Vertex*>::iterator v = C->begin(); v != C->end(); v++)
                        CSet.insert(*v);

                    VertexSet SoC = SetHelper::Union(S, CSet);
                    VertexSet SoWaC = SetHelper::Union(S, SetHelper::Intersection(W,CSet));
                    DirectedTreeDecomposition* d = FindDirectedTreeDecomposition(G, SoC, SoWaC, k);
                    if(d == NULL) // treewidth is larger than k
                    {
                        delete result;
                        return NULL;
                    }
                    result->Children.push_back(d);
                }
                result->Bag = SetHelper::Union(W,S);

                return result;
            }



            DirectedTreeDecomposition* AlgorithmDIRECTEDTREEDECOMPOSITION::FindDirectedTreeDecomposition(Graph& G, size_t k)
            {
                VertexIterator v = G.BeginVertices();

                VertexSet W;
                for(size_t i = 0; i < 2*k+2 && v != G.EndVertices(); i++, v++)
                    W.insert(*v);

                VertexSet V;
                for(v = G.BeginVertices(); v != G.EndVertices(); v++)
                    V.insert(*v);

                return FindDirectedTreeDecomposition(G,V,W,k);
            }



            DirectedTreeDecomposition* AlgorithmDIRECTEDTREEDECOMPOSITION::FindDirectedTreeDecomposition(Graph& G)
            {
                for(size_t k = 1; ; k++)
                {
                    cerr << "k = " << k << endl; cerr.flush();
                    DirectedTreeDecomposition* d = FindDirectedTreeDecomposition(G, k);
                    if(d != NULL)
                        return d;
                }
            }


            void AlgorithmDIRECTEDTREEDECOMPOSITION::PrintDirectedTreeDecomposition(DirectedTreeDecomposition* d, size_t indent)
            {
                for(size_t i = 0; i < indent; i++)
                    cout << "   ";
                for(VertexIterator v = d->Bag.begin(); v != d->Bag.end(); v++)
                    cout << (*v)->GetLabel() << " ";
                cout << "\n";
                for(vector<DirectedTreeDecomposition*>::iterator c = d->Children.begin(); c != d->Children.end(); c++)
                    PrintDirectedTreeDecomposition(*c, indent+1);
            }

            void AlgorithmDIRECTEDTREEDECOMPOSITION::Run(Graph& G, std::vector<std::string> parameters)
            {
                if(parameters.size() < 1)
                    throw "directed treewidth algorithm needs 1 parameter (result name)";
                string ResultName = parameters[0];

                DirectedTreeDecomposition* d = FindDirectedTreeDecomposition(G);
                if(d != NULL)
                {
                    PrintDirectedTreeDecomposition(d, 0);
                    delete d;
                }
            }

        }
    }
}
