
#include "../../../Headers/logic/ComputationTreeLogic/ctl.h"
#include "../../../Headers/logic/ComputationTreeLogic/ctl_lexer.h"
using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Logic;
#include "ctl_parser.hpp"


    FactoryRegistrator<ModelChecker> CTLModelChecker::ModelCheckerCTLRegistrator(
        &ModelChecker::ModelCheckerFactory, "ctl", new DefaultInstantiator<ModelChecker, CTLModelChecker>(
            "ctl", "Computation Tree Logic","http://en.wikipedia.org/wiki/Computation_tree_logic"));


    Formula* CTLModelChecker::Parse(istream& is)
    {
        OpenGraphtheory::Logic::CTLLexer l(is);
        CTLFormula* result;
        CTLParser p(l, &result);
        if(p.parse() == 0)
        {
            return result;
        }
        else
        {
            return NULL;
        }
    }


    void CTLFormula::Interpret(Graph &G, list<std::string> parameters)
    {
        if(parameters.size() > 0)
        {
            VertexSet result = Interpretation(G);
            G.AddVertexSet(result, *(parameters.begin()));
        }
    }

    // -------------------------------------------------------------------------

    CTL_True::CTL_True()
    {
    }

    CTL_True::~CTL_True()
    {
    }

    Formula* CTL_True::Clone()
    {
        return new CTL_True();
    }

    VertexSet CTL_True::Interpretation(Graph &G)
    {
        VertexSet result;
        for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            result.insert(*v);
        return result;
    }

    // -------------------------------------------------------------------------

    CTL_False::CTL_False()
    {
    }

    CTL_False::~CTL_False()
    {
    }

    Formula* CTL_False::Clone()
    {
        return new CTL_False();
    }

    VertexSet CTL_False::Interpretation(Graph &G)
    {
        VertexSet result;
        return result;
    }

    // -------------------------------------------------------------------------

    CTL_Atomic::CTL_Atomic(std::string* atomic_test_description)
    {
        test_description = atomic_test_description;
    }

    CTL_Atomic::~CTL_Atomic()
    {
        delete test_description;
    }

    Formula* CTL_Atomic::Clone()
    {
        return new CTL_Atomic(new std::string(*test_description));
    }


    bool CTL_Atomic::Satisfies(Vertex* v)
    {
        Attribute *attr = v->GetAttribute(*test_description);
        BoolAttribute *battr = dynamic_cast<BoolAttribute*>(attr);
        if(battr != NULL)
            return battr->Value;

        string label = string(",") + v->GetLabel() + string(",");
        if( label.find( string(",") + *test_description + string(",") ) != label.npos )
            return true;

        return false;
    }

    VertexSet CTL_Atomic::Interpretation(Graph &G)
    {
        VertexSet result;
        for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            if(Satisfies(*v))
                result.insert(*v);
        return result;
    }



    // -------------------------------------------------------------------------

    CTL_Not::CTL_Not(CTLFormula* phi)
    {
        if(phi == NULL)
            throw "CTL_Not::CTL_Not parameter must not be NULL";
        this->phi = phi;
    }

    CTL_Not::~CTL_Not()
    {
        delete phi;
    }

    Formula* CTL_Not::Clone()
    {
        return new CTL_Not(static_cast<CTLFormula*>(phi->Clone()));
    }

    VertexSet CTL_Not::Interpretation(Graph &G)
    {
        VertexSet phiresult = phi->Interpretation(G);
        VertexSet result;
        for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            if(!phiresult.contains(*v))
                result.insert(*v);
        return result;
    }

    // -------------------------------------------------------------------------

    CTL_And::CTL_And(CTLFormula* phi1, CTLFormula* phi2)
    {
        if(phi1 == NULL || phi2 == NULL)
            throw "CTL_And::CTL_And parameters must not be NULL";
        this->phi1 = phi1;
        this->phi2 = phi2;
    }

    CTL_And::~CTL_And()
    {
        delete phi1;
        delete phi2;
    }

    Formula* CTL_And::Clone()
    {
        return new CTL_And(static_cast<CTLFormula*>(phi1->Clone()),
                           static_cast<CTLFormula*>(phi2->Clone()));
    }

    VertexSet CTL_And::Interpretation(Graph &G)
    {
        VertexSet phi1result = phi1->Interpretation(G);
        VertexSet phi2result = phi2->Interpretation(G);
        return phi1result.intersection(phi2result);
    }

    // -------------------------------------------------------------------------

    CTL_Or::CTL_Or(CTLFormula* phi1, CTLFormula* phi2)
    {
        if(phi1 == NULL || phi2 == NULL)
            throw "CTL_Or::CTL_Or parameters must not be NULL";
        this->phi1 = phi1;
        this->phi2 = phi2;
    }

    CTL_Or::~CTL_Or()
    {
        delete phi1;
        delete phi2;
    }

    Formula* CTL_Or::Clone()
    {
        return new CTL_Or(static_cast<CTLFormula*>(phi1->Clone()), static_cast<CTLFormula*>(phi2->Clone()));
    }

    VertexSet CTL_Or::Interpretation(Graph &G)
    {
        return phi1->Interpretation(G) + phi2->Interpretation(G);
    }

    // -------------------------------------------------------------------------

    CTL_ExistsNext::CTL_ExistsNext(CTLFormula* phi)
    {
        if(phi == NULL)
            throw "CTL_ExistsNext::CTL_ExistsNext parameter must not be NULL";
        this->phi = phi;
    }

    CTL_ExistsNext::~CTL_ExistsNext()
    {
        delete phi;
    }

    Formula* CTL_ExistsNext::Clone()
    {
        return new CTL_ExistsNext(static_cast<CTLFormula*>(phi->Clone()));
    }

    VertexSet CTL_ExistsNext::Interpretation(Graph &G)
    {
        VertexSet phiresult = phi->Interpretation(G);
        VertexSet result;
        for(VertexIterator v = phiresult.begin(); v != phiresult.end(); v++)
        {
            VertexSet vPredecessors = (*v)->CollectNeighbors(1,0,1,0,0,0,1,0,1);
            for(VertexIterator n = vPredecessors.begin(); n != vPredecessors.end(); n++)
                result.insert(*n);
        }
        return result;
    }

    // -------------------------------------------------------------------------

    CTL_ExistsUntil::CTL_ExistsUntil(CTLFormula* phi1, CTLFormula* phi2)
    {
        if(phi1 == NULL || phi2 == NULL)
            throw "CTL_ExistsUntil::CTL_ExistsUntil parameters must not be NULL";
        this->phi1 = phi1;
        this->phi2 = phi2;
    }

    CTL_ExistsUntil::~CTL_ExistsUntil()
    {
        delete phi1;
        delete phi2;
    }

    Formula* CTL_ExistsUntil::Clone()
    {
        return new CTL_ExistsUntil(static_cast<CTLFormula*>(phi1->Clone()), static_cast<CTLFormula*>(phi2->Clone()));
    }

    VertexSet CTL_ExistsUntil::Interpretation(Graph &G)
    {
        VertexSet phi1result = phi1->Interpretation(G);
        VertexSet phi2result = phi2->Interpretation(G);
        VertexSet last = phi2result;
        VertexSet next;

        while(last.size() > 0)
        {
            for(VertexIterator l = last.begin(); l != last.end(); l++)
            {
                VertexSet lPredecessors = (*l)->CollectNeighbors(1,0,1,0,0,0,1,0,1);
                for(VertexIterator lpred = lPredecessors.begin(); lpred != lPredecessors.end(); lpred++)
                    if(phi1result.contains(*lpred)) // if lpred satisfies phi1
                        if(!phi2result.contains(*lpred) && !last.contains(*lpred)) // and lpred is a new element
                            next.insert(*lpred);
            }

            phi2result += next;
            last = next;
            next.clear();
        }

        return phi2result;
    }

    // -------------------------------------------------------------------------

    CTL_ExistsGlobally::CTL_ExistsGlobally(CTLFormula* phi)
    {
        if(phi == NULL)
            throw "CTL_ExistsGlobally::CTL_ExistsGlobally parameter must not be NULL";
        this->phi = phi;
    }

    CTL_ExistsGlobally::~CTL_ExistsGlobally()
    {
        delete phi;
    }

    Formula* CTL_ExistsGlobally::Clone()
    {
        return new CTL_ExistsGlobally(static_cast<CTLFormula*>(phi->Clone()));
    }

    VertexSet CTL_ExistsGlobally::Interpretation(Graph &G)
    {
        VertexSet phiresult = phi->Interpretation(G);
        VertexSet lastremoved;
        VertexSet nextremoved;
        for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            if(!phiresult.contains(*v))
                lastremoved.insert(*v);
        // lastremoved = V - phiresult

        while(lastremoved.size() > 0)
        {
            VertexSet RemovalCandidates;
            for(VertexIterator v = lastremoved.begin(); v != lastremoved.end(); v++)
            {
                VertexSet vPred = (*v)->CollectNeighbors(1,0,1,0,0,0,1,0,1);
                for(VertexIterator p = vPred.begin(); p != vPred.end(); p++)
                    if(phiresult.contains(*p))
                        RemovalCandidates.insert(*p);
            }

            for(VertexIterator v = RemovalCandidates.begin(); v != RemovalCandidates.end(); v++)
            {
                VertexSet vSucc = (*v)->CollectNeighbors(1,1,0,1,1,0,0,0,0);
                bool MustBeRemoved = true;
                for(VertexIterator s = vSucc.begin(); MustBeRemoved && s != vSucc.end(); s++)
                    if(phiresult.find(*s) != phiresult.end()) // v still has a successor in phiresult
                        MustBeRemoved = false;

                if(MustBeRemoved)
                {
                    phiresult.erase(*v);
                    nextremoved.insert(*v);
                }
            }

            lastremoved = nextremoved;
            nextremoved.clear();
        }

        return phiresult;
    }

    // -------------------------------------------------------------------------

    CTL_Forwarder::CTL_Forwarder()
    {
        forwarded = NULL;
    }

    CTL_Forwarder::~CTL_Forwarder()
    {
        if(forwarded != NULL)
            delete forwarded;
    }

    VertexSet CTL_Forwarder::Interpretation(Graph& G)
    {
        return forwarded->Interpretation(G);
    }

    Formula* CTL_Forwarder::Clone()
    {
        CTL_Forwarder* result = new CTL_Forwarder();
        result->forwarded = static_cast<CTLFormula*>(forwarded->Clone());
        return result;
    }


    CTL_ExistsFinally::CTL_ExistsFinally(CTLFormula* phi)
    {
        // EF phi = E true U phi
        forwarded = new CTL_ExistsUntil( new CTL_True(), phi);
    }

    CTL_AlwaysNext::CTL_AlwaysNext(CTLFormula* phi)
    {
        // AX phi = not EX not phi
        forwarded = new CTL_Not(new CTL_ExistsNext(new CTL_Not(phi)));
    }

    CTL_AlwaysGlobally::CTL_AlwaysGlobally(CTLFormula* phi)
    {
        // AG phi = not EF not phi
        forwarded = new CTL_Not(new CTL_ExistsFinally(new CTL_Not(phi)));
    }

    CTL_AlwaysFinally::CTL_AlwaysFinally(CTLFormula* phi)
    {
        // AF phi = not EG not phi
        forwarded = new CTL_Not(new CTL_ExistsGlobally(new CTL_Not(phi)));
    }

    CTL_AlwaysUntil::CTL_AlwaysUntil(CTLFormula* phi1, CTLFormula* phi2)
    {
        // A phi U psi = (not E(not psi U (not phi and not psi))) AND not EG not psi

        forwarded =
            new CTL_And(
                new CTL_Not(new CTL_ExistsUntil(
                    new CTL_Not(phi2)
                    ,
                    new CTL_Not(new CTL_Or(
                        phi1,
                        static_cast<CTLFormula*>(phi2->Clone())
                    ))
                ))

                ,

                new CTL_Not(new CTL_ExistsGlobally(
                        new CTL_Not(static_cast<CTLFormula*>(phi2->Clone()))
                ))
            );
    }








