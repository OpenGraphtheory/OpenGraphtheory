
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


    void CTLFormula::Interpret(Graph &G)
    {
        string resultname = "ctl_model";
        set<Graph::VertexIterator> result = Interpretation(G);
        G.AddVertexSet(result, resultname);
    }

    // -------------------------------------------------------------------------

    CTL_True::CTL_True()
    {
    }

    CTL_True::~CTL_True()
    {
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

    bool CTL_Atomic::Satisfies(Graph::VertexIterator v)
    {
        string label = v.GetLabel();

        if(label.find(string(",") + *test_description + ",") != label.npos
            || label.find(*test_description + ",") == 0
            || label.find(string(",") + *test_description) == label.length() - test_description->length())
        {
            return true;
        }

        if(v.Attributes().HasAttribute(*test_description))
        {
            Attribute *attr = v.Attributes().GetAttribute(*test_description);
            BoolAttribute *battr = dynamic_cast<BoolAttribute*>(attr);
            if(battr != NULL)
                return battr->Value;
        }

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

    set<Graph::VertexIterator> CTL_ExistsNext::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phiresult = phi->Interpretation(G);
        set<Graph::VertexIterator> result;
        for(set<Graph::VertexIterator>::iterator v = phiresult.begin(); v != phiresult.end(); v++)
        {
            Graph::VertexIterator vi = *v;
            set<Graph::VertexIterator> vPredecessors = vi.CollectNeighbors(1,1,0,1,1,0,0,0,0);
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

    set<Graph::VertexIterator> CTL_ExistsUntil::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phi1result = phi1->Interpretation(G);
        set<Graph::VertexIterator> phi2result = phi2->Interpretation(G);
        set<Graph::VertexIterator> last = phi2result;
        set<Graph::VertexIterator> next;

        do
        {
            for(set<Graph::VertexIterator>::iterator l = last.begin(); l != last.end(); l++)
            {
                Graph::VertexIterator li = *l;
                set<Graph::VertexIterator> lPredecessors = li.CollectNeighbors(1,1,0,1,1,0,0,0,0);
                for(set<Graph::VertexIterator>::iterator lpred = lPredecessors.begin(); lpred != lPredecessors.end(); lpred++)
                    if(phi1result.find(*lpred) != phi1result.end()) // if lpred satisfies phi1
                        if(phi2result.find(*lpred) == phi2result.end() && last.find(*lpred) == last.end()) // and lpred is a new element
                            next.insert(*lpred);
            }

            for(set<Graph::VertexIterator>::iterator n = next.begin(); n != next.end(); n++)
                phi2result.insert(*n);
            last = next;
            next.clear();
        } while(last.size() > 0);

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

    set<Graph::VertexIterator> CTL_ExistsGlobally::Interpretation(Graph &G)
    {
        set<Graph::VertexIterator> phiresult = phi->Interpretation(G);
        for(set<Graph::VertexIterator>::iterator v = phiresult.begin(); v != phiresult.end(); v++)
            phiresult.insert(*v);
        return phiresult;
    }

    // -------------------------------------------------------------------------

    CTL_Forwarder::~CTL_Forwarder()
    {

    }

    set<Graph::VertexIterator> CTL_Forwarder::Interpretation(Graph& G)
    {
        return forwarded->Interpretation(G);
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

    /*
    CTL_AlwaysUntil::CTL_AlwaysUntil(CTLFormula* phi1, CTLFormula* phi2)
    {
        // AG phi = not EF not phi
        forwarded = new CTL_Not(new CTL_ExistsFinally(new CTL_Not(phi)));
    }
    */








