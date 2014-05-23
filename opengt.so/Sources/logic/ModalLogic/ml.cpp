
#include "../../../Headers/logic/ModalLogic/ml.h"
#include "../../../Headers/logic/ModalLogic/ml_lexer.h"
using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Logic;
#include "ml_parser.hpp"


    FactoryRegistrator<ModelChecker> MLModelChecker::ModelCheckerMLRegistrator(
        &ModelChecker::ModelCheckerFactory, "ml", new DefaultInstantiator<ModelChecker, MLModelChecker>(
            "ml", "Modal Logic","http://en.wikipedia.org/wiki/Modal_logic"));


    Formula* MLModelChecker::Parse(istream& is)
    {
        OpenGraphtheory::Logic::MLLexer l(is);
        MLFormula* result;
        MLParser p(l, &result);
        if(p.parse() == 0)
        {
            return result;
        }
        else
        {
            return NULL;
        }
    }


    void MLFormula::Interpret(Graph &G, list<std::string> parameters)
    {
        if(parameters.size() > 0)
        {
            VertexSet result = Interpretation(G);
            G.AddVertexSet(result, *(parameters.begin()));
        }
    }

    // -------------------------------------------------------------------------

    ML_True::ML_True()
    {
    }

    ML_True::~ML_True()
    {
    }

    Formula* ML_True::Clone()
    {
        return new ML_True();
    }

    VertexSet ML_True::Interpretation(Graph &G)
    {
        VertexSet result;
        for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            result.insert(*v);
        return result;
    }

    // -------------------------------------------------------------------------

    ML_False::ML_False()
    {
    }

    ML_False::~ML_False()
    {
    }

    Formula* ML_False::Clone()
    {
        return new ML_False();
    }

    VertexSet ML_False::Interpretation(Graph &G)
    {
        VertexSet result;
        return result;
    }

    // -------------------------------------------------------------------------

    ML_Atomic::ML_Atomic(std::string* atomic_test_description)
    {
        test_description = atomic_test_description;
    }

    ML_Atomic::~ML_Atomic()
    {
        delete test_description;
    }

    Formula* ML_Atomic::Clone()
    {
        return new ML_Atomic(new std::string(*test_description));
    }


    bool ML_Atomic::Satisfies(Vertex* v)
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

    VertexSet ML_Atomic::Interpretation(Graph &G)
    {
        VertexSet result;
        for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            if(this->Satisfies(*v))
                result.insert(*v);
        return result;
    }



    // -------------------------------------------------------------------------

    ML_Not::ML_Not(MLFormula* phi)
    {
        if(phi == NULL)
            throw "ML_Not::ML_Not parameter must not be NULL";
        this->phi = phi;
    }

    ML_Not::~ML_Not()
    {
        delete phi;
    }

    Formula* ML_Not::Clone()
    {
        return new ML_Not(static_cast<MLFormula*>(phi->Clone()));
    }

    VertexSet ML_Not::Interpretation(Graph &G)
    {
        VertexSet phiresult = phi->Interpretation(G);
        VertexSet result;
        for(VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            if(!phiresult.contains(*v))
                result.insert(*v);
        return result;
    }

    // -------------------------------------------------------------------------

    ML_And::ML_And(MLFormula* phi1, MLFormula* phi2)
    {
        if(phi1 == NULL || phi2 == NULL)
            throw "ML_And::ML_And parameters must not be NULL";
        this->phi1 = phi1;
        this->phi2 = phi2;
    }

    ML_And::~ML_And()
    {
        delete phi1;
        delete phi2;
    }

    Formula* ML_And::Clone()
    {
        return new ML_And(static_cast<MLFormula*>(phi1->Clone()),
                           static_cast<MLFormula*>(phi2->Clone()));
    }

    VertexSet ML_And::Interpretation(Graph &G)
    {
        VertexSet phi1result = phi1->Interpretation(G);
        VertexSet phi2result = phi2->Interpretation(G);
        return phi1result.intersection(phi2result);
    }

    // -------------------------------------------------------------------------

    ML_Or::ML_Or(MLFormula* phi1, MLFormula* phi2)
    {
        if(phi1 == NULL || phi2 == NULL)
            throw "ML_Or::ML_Or parameters must not be NULL";
        this->phi1 = phi1;
        this->phi2 = phi2;
    }

    ML_Or::~ML_Or()
    {
        delete phi1;
        delete phi2;
    }

    Formula* ML_Or::Clone()
    {
        return new ML_Or(static_cast<MLFormula*>(phi1->Clone()), static_cast<MLFormula*>(phi2->Clone()));
    }

    VertexSet ML_Or::Interpretation(Graph &G)
    {
        return phi1->Interpretation(G) + phi2->Interpretation(G);
    }

    // -------------------------------------------------------------------------

    ML_Box::ML_Box(MLFormula* phi)
    {
        if(phi == NULL)
            throw "ML_Box::ML_Box parameter must not be NULL";
        this->phi = phi;
    }

    ML_Box::~ML_Box()
    {
        delete phi;
    }

    Formula* ML_Box::Clone()
    {
        return new ML_Box(static_cast<MLFormula*>(phi->Clone()));
    }

    VertexSet ML_Box::Interpretation(Graph &G)
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

    ML_Forwarder::ML_Forwarder()
    {
        forwarded = NULL;
    }

    ML_Forwarder::~ML_Forwarder()
    {
        if(forwarded != NULL)
            delete forwarded;
    }

    VertexSet ML_Forwarder::Interpretation(Graph& G)
    {
        return forwarded->Interpretation(G);
    }

    Formula* ML_Forwarder::Clone()
    {
        ML_Forwarder* result = new ML_Forwarder();
        result->forwarded = static_cast<MLFormula*>(forwarded->Clone());
        return result;
    }

    ML_Diamond::ML_Diamond(MLFormula* phi)
    {
        // Diamond phi = not Box not phi
        forwarded = new ML_Not(new ML_Box(new ML_Not(phi)));
    }



