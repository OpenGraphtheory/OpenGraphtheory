
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
            set<Graph::VertexIterator> result = Interpretation(G);
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

    set<Graph::VertexIterator> CTL_True::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> result;
        for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            result.insert(v);
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

    set<Graph::VertexIterator> CTL_False::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> result;
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


    bool CTL_Atomic::Satisfies(Graph::VertexIterator v)
    {
        if(v.Attributes().HasAttribute(*test_description))
        {
            Attribute *attr = v.Attributes().GetAttribute(*test_description);
            BoolAttribute *battr = dynamic_cast<BoolAttribute*>(attr);
            if(battr != NULL)
                return battr->Value;
        }

        string label = string(",")+v.GetLabel()+string(",");
        if( label.find( string(",")+*test_description+string(",") ) != label.npos )
            return true;

        return false;
    }

    set<Graph::VertexIterator> CTL_Atomic::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> result;
        for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            if(Satisfies(v))
                result.insert(v);
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

    set<Graph::VertexIterator> CTL_Not::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phiresult = phi->Interpretation(G);
        set<Graph::VertexIterator> result;
        for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            if(phiresult.find(v) == phiresult.end())
                result.insert(v);
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

    set<Graph::VertexIterator> CTL_And::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phi1result = phi1->Interpretation(G);
        set<Graph::VertexIterator> phi2result = phi2->Interpretation(G);
        set<Graph::VertexIterator> result;
        for(set<Graph::VertexIterator>::iterator v = phi1result.begin(); v != phi1result.end(); v++)
            if(phi2result.find(*v) != phi2result.end())
                result.insert(*v);
        return result;
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

    set<Graph::VertexIterator> CTL_Or::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phi1result = phi1->Interpretation(G);
        set<Graph::VertexIterator> phi2result = phi2->Interpretation(G);
        for(set<Graph::VertexIterator>::iterator v = phi2result.begin(); v != phi2result.end(); v++)
            phi1result.insert(*v);
        return phi1result;
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

    set<Graph::VertexIterator> CTL_ExistsNext::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phiresult = phi->Interpretation(G);
        set<Graph::VertexIterator> result;
        for(set<Graph::VertexIterator>::iterator v = phiresult.begin(); v != phiresult.end(); v++)
        {
            Graph::VertexIterator vi = *v;
            set<Graph::VertexIterator> vPredecessors = vi.CollectNeighbors(1,0,1,0,0,0,1,0,1);
            for(set<Graph::VertexIterator>::iterator n = vPredecessors.begin(); n != vPredecessors.end(); n++)
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

    set<Graph::VertexIterator> CTL_ExistsUntil::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phi1result = phi1->Interpretation(G);
        set<Graph::VertexIterator> phi2result = phi2->Interpretation(G);
        set<Graph::VertexIterator> last = phi2result;
        set<Graph::VertexIterator> next;

        while(last.size() > 0)
        {
            for(set<Graph::VertexIterator>::iterator l = last.begin(); l != last.end(); l++)
            {
                Graph::VertexIterator li = *l;
                set<Graph::VertexIterator> lPredecessors = li.CollectNeighbors(1,0,1,0,0,0,1,0,1);
                for(set<Graph::VertexIterator>::iterator lpred = lPredecessors.begin(); lpred != lPredecessors.end(); lpred++)
                    if(phi1result.find(*lpred) != phi1result.end()) // if lpred satisfies phi1
                        if(phi2result.find(*lpred) == phi2result.end() && last.find(*lpred) == last.end()) // and lpred is a new element
                            next.insert(*lpred);
            }

            for(set<Graph::VertexIterator>::iterator n = next.begin(); n != next.end(); n++)
                phi2result.insert(*n);
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

    set<Graph::VertexIterator> CTL_ExistsGlobally::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phiresult = phi->Interpretation(G);
        set<Graph::VertexIterator> lastremoved;
        set<Graph::VertexIterator> nextremoved;
        for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            if(phiresult.find(v) == phiresult.end())
                lastremoved.insert(v);
        // lastremoved = V - phiresult

        while(lastremoved.size() > 0)
        {
            set<Graph::VertexIterator> RemovalCandidates;
            for(set<Graph::VertexIterator>::iterator v = lastremoved.begin(); v != lastremoved.end(); v++)
            {
                Graph::VertexIterator vi = *v;
                set<Graph::VertexIterator> vPred = vi.CollectNeighbors(1,0,1,0,0,0,1,0,1);
                for(set<Graph::VertexIterator>::iterator p = vPred.begin(); p != vPred.end(); p++)
                    if(phiresult.find(*p) != phiresult.end())
                        RemovalCandidates.insert(*p);
            }

            for(set<Graph::VertexIterator>::iterator v = RemovalCandidates.begin(); v != RemovalCandidates.end(); v++)
            {
                Graph::VertexIterator vi = *v;
                set<Graph::VertexIterator> vSucc = vi.CollectNeighbors(1,1,0,1,1,0,0,0,0);
                bool MustBeRemoved = true;
                for(set<Graph::VertexIterator>::iterator s = vSucc.begin(); MustBeRemoved && s != vSucc.end(); s++)
                    if(phiresult.find(*s) != phiresult.end()) // v still has a successor in phiresult
                        MustBeRemoved = false;

                if(MustBeRemoved)
                {
                    phiresult.erase(*v);
                    nextremoved.insert(vi);
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

    }

    set<Graph::VertexIterator> CTL_Forwarder::Interpretation(Graph& G)
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








