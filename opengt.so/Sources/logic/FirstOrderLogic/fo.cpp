
#include "../../../Headers/logic/FirstOrderLogic/fo.h"
#include "../../../Headers/logic/FirstOrderLogic/fo_lexer.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Logic;

#include "fo_parser.hpp"


namespace OpenGraphtheory
{
    namespace Logic
    {

        FactoryRegistrator<ModelChecker> FOModelChecker::ModelCheckerFORegistrator(
            &ModelChecker::ModelCheckerFactory, "fo", new DefaultInstantiator<ModelChecker, FOModelChecker>(
                "fo", "First-Order Logic","http://en.wikipedia.org/wiki/First-order_logic"));


        Formula* FOModelChecker::Parse(istream& is)
        {
            OpenGraphtheory::Logic::FOLexer l(is);
            FOFormula* result;
            FOParser p(l, &result);
            if(p.parse() == 0)
            {
                return result;
            }
            else
            {
                return NULL;
            }
        }


        void FOFormula::Interpret(Graph &G, list<string> parameters)
        {
            map<string, Vertex*> VariableAssignment;
            bool result = Interpretation(G, VariableAssignment);
            exit(result ? 0 : 1);
        }

        // ---------------------------------------------------------------------

        FO_True::FO_True()
        {
        }

        FO_True::~FO_True()
        {
        }

        Formula* FO_True::Clone()
        {
            return new FO_True();
        }

        bool FO_True::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            return true;
        }

        // ---------------------------------------------------------------------

        FO_False::FO_False()
        {
        }

        FO_False::~FO_False()
        {
        }

        Formula* FO_False::Clone()
        {
            return new FO_False();
        }

        bool FO_False::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            return false;
        }

        // ---------------------------------------------------------------------

        FO_Eq::FO_Eq(string* param1, string* param2)
        {
            if(param1 == NULL || param2 == NULL)
                throw "FO_Eq::FO_Eq parameter must not be NULL";
            this->param1 = param1;
            this->param2 = param2;
        }

        FO_Eq::~FO_Eq()
        {
            delete param1;
            delete param2;
        }

        Formula* FO_Eq::Clone()
        {
            return new FO_Eq(new string(*param1), new string(*param2));
        }

        bool FO_Eq::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            return VariableAssignment[*param1] == VariableAssignment[*param2];
        }

        // ---------------------------------------------------------------------

        FO_Neq::FO_Neq(string* param1, string* param2)
        {
            if(param1 == NULL || param2 == NULL)
                throw "FO_Neq::FO_Neq parameter must not be NULL";
            this->param1 = param1;
            this->param2 = param2;
        }

        FO_Neq::~FO_Neq()
        {
            delete param1;
            delete param2;
        }

        Formula* FO_Neq::Clone()
        {
            return new FO_Neq(new string(*param1), new string(*param2));
        }

        bool FO_Neq::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            return VariableAssignment[*param1] != VariableAssignment[*param2];
        }


        // ---------------------------------------------------------------------

        FO_Parameter::FO_Parameter(string* variablename, FO_Parameter* Next)
        {
            this->Next = Next;
            this->variablename = variablename;
        }

        FO_Parameter::~FO_Parameter()
        {
            if(Next != NULL)
                delete Next;
            delete variablename;
        }

        string FO_Parameter::GetVariableName()
        {
            return *variablename;
        }

        FO_Parameter* FO_Parameter::GetNext()
        {
            return Next;
        }

        // ---------------------------------------------------------------------

        FO_Atomic::FO_Atomic(string* relation, FO_Parameter* parameters)
        {
            this->relation = relation;
            for(FO_Parameter* run = parameters; run != NULL; run = run->GetNext())
                this->parameters.push_back(run->GetVariableName());
            delete parameters;
        }

        FO_Atomic::~FO_Atomic()
        {
            delete relation;
        }

        Formula* FO_Atomic::Clone()
        {
            FO_Atomic* clone = new FO_Atomic(new string(*relation), NULL);
            for(list<string>::iterator i = parameters.begin(); i != parameters.end(); i++)
                clone->parameters.push_back(*i);
            return clone;
        }


        bool FO_Atomic::EdgeMatchesRelation(Edge* e, string relation)
        {
            if(relation == "e" || relation == "E" || relation == "arc")
                return true;

            Attribute *attr = e->GetAttribute(relation);
             BoolAttribute *battr = dynamic_cast<BoolAttribute*>(attr);
            if(battr != NULL)
                return battr->Value;

            string label = string(",") + e->GetLabel() + string(",");
            if( label.find( string(",") + relation + string(",") ) != label.npos )
                return true;

            return false;
        }

        bool FO_Atomic::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            list<Vertex*> iParameters;
            for(list<string>::iterator i = parameters.begin(); i != parameters.end(); i++)
            {
                if(VariableAssignment.find(*i) == VariableAssignment.end())
                    throw "Undeclared Variable";
                iParameters.push_back(VariableAssignment[*i]);
            }

            Vertex* first = *(iParameters.begin());
            EdgeSet IncidentEdges = first->CollectIncidentEdges(1,1,1);
            for(EdgeIterator ei = IncidentEdges.begin(); ei != IncidentEdges.end(); ei++)
            {
                Edge* e = *ei;
                if(!EdgeMatchesRelation(e, *relation))
                    continue;

                // compare vertices in edge to vertices in relation parameters
                list<Vertex*>::iterator param = iParameters.begin();
                bool CurrentEdgeSatisfiesQuery = true;
                for(VertexEdgeConnectionIterator conn = e->BeginConnections(); conn != e->EndConnections(); conn++)
                {
                    if(param == iParameters.end()) // different number of elements
                    {
                        CurrentEdgeSatisfiesQuery = false;
                        break;
                    }

                    if(*param != (*conn)->GetVertex())
                    {
                        CurrentEdgeSatisfiesQuery = false;
                        break;
                    }

                    param++;
                }

                if(CurrentEdgeSatisfiesQuery && param == iParameters.end())
                    return true;
            }

            return false;
        }



        // ---------------------------------------------------------------------

        FO_Not::FO_Not(FOFormula* phi)
        {
            if(phi == NULL)
                throw "FO_Not::FO_Not parameter must not be NULL";
            this->phi = phi;
        }

        FO_Not::~FO_Not()
        {
            delete phi;
        }

        Formula* FO_Not::Clone()
        {
            return new FO_Not(static_cast<FOFormula*>(phi->Clone()));
        }

        bool FO_Not::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            return !(phi->Interpretation(G, VariableAssignment));
        }

        // ---------------------------------------------------------------------

        FO_And::FO_And(FOFormula* phi1, FOFormula* phi2)
        {
            if(phi1 == NULL || phi2 == NULL)
                throw "FO_And::FO_And parameters must not be NULL";
            this->phi1 = phi1;
            this->phi2 = phi2;
        }

        FO_And::~FO_And()
        {
            delete phi1;
            delete phi2;
        }

        Formula* FO_And::Clone()
        {
            return new FO_And(static_cast<FOFormula*>(phi1->Clone()),
                            static_cast<FOFormula*>(phi2->Clone()));
        }

        bool FO_And::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            return phi1->Interpretation(G, VariableAssignment)
                && phi2->Interpretation(G, VariableAssignment);
        }

        // ---------------------------------------------------------------------

        FO_Or::FO_Or(FOFormula* phi1, FOFormula* phi2)
        {
            if(phi1 == NULL || phi2 == NULL)
                throw "FO_Or::FO_Or parameters must not be NULL";
            this->phi1 = phi1;
            this->phi2 = phi2;
        }

        FO_Or::~FO_Or()
        {
            delete phi1;
            delete phi2;
        }

        Formula* FO_Or::Clone()
        {
            return new FO_Or(static_cast<FOFormula*>(phi1->Clone()), static_cast<FOFormula*>(phi2->Clone()));
        }

        bool FO_Or::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            return phi1->Interpretation(G, VariableAssignment)
                || phi2->Interpretation(G, VariableAssignment);
        }

        // ---------------------------------------------------------------------

        FO_Forall::FO_Forall(string* variablename, FOFormula* phi)
        {
            if(phi == NULL || variablename == NULL)
                throw "FO_Forall::FO_Forall parameters must not be NULL";
            this->variablename = variablename;
            this->phi = phi;
        }

        FO_Forall::~FO_Forall()
        {
            delete phi;
            delete variablename;
        }

        Formula* FO_Forall::Clone()
        {
            return new FO_Forall(new string(*variablename), static_cast<FOFormula*>(phi->Clone()));
        }

        bool FO_Forall::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            bool VariableCovered = false;
            Vertex* bak;
            if(VariableAssignment.find(*variablename) != VariableAssignment.end())
            {
                bak = VariableAssignment[*variablename];
                VariableCovered = true;
            }

            bool currentresult = true;
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices() && currentresult == true; v++)
            {
                VariableAssignment[*variablename] = *v;
                if(phi->Interpretation(G, VariableAssignment) == false)
                    currentresult = false;
            }

            if(VariableCovered)
                VariableAssignment[*variablename] = bak;
            else
                VariableAssignment.erase(*variablename);

            return currentresult;
        }

        // ---------------------------------------------------------------------

        FO_Exists::FO_Exists(string* variablename, FOFormula* phi)
        {
            if(phi == NULL || variablename == NULL)
                throw "FO_Exists::FO_Exists parameter must not be NULL";
            this->variablename = variablename;
            this->phi = phi;
        }

        FO_Exists::~FO_Exists()
        {
            delete phi;
            delete variablename;
        }

        Formula* FO_Exists::Clone()
        {
            return new FO_Exists(new string(*variablename), static_cast<FOFormula*>(phi->Clone()));
        }

        bool FO_Exists::Interpretation(Graph &G, map<string, Vertex*> &VariableAssignment)
        {
            bool VariableCovered = false;
            Vertex* bak;
            if(VariableAssignment.find(*variablename) != VariableAssignment.end())
            {
                bak = VariableAssignment[*variablename];
                VariableCovered = true;
            }

            bool currentresult = false;
            for(VertexIterator v = G.BeginVertices(); v != G.EndVertices() && currentresult == false; v++)
            {
                VariableAssignment[*variablename] = *v;
                if(phi->Interpretation(G, VariableAssignment) == true)
                    currentresult = true;
            }

            if(VariableCovered)
                VariableAssignment[*variablename] = bak;
            else
                VariableAssignment.erase(*variablename);
            return currentresult;
        }


    } // namespace Logic
} // namespace OpenGraphtheory

