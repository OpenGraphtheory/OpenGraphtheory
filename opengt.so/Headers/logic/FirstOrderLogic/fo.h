
#ifndef __OPENGRAPHTHEORY_LOGIC_FIRSTORDERLOGIC_FO_H
    #define __OPENGRAPHTHEORY_LOGIC_FIRSTORDERLOGIC_FO_H

    #include<string>
	#include<map>
    #include "../logic.h"

    namespace OpenGraphtheory
    {
        namespace Logic
        {
            class FOModelChecker : public ModelChecker
            {
                protected:
                    static FactoryRegistrator<ModelChecker> ModelCheckerFORegistrator;
                public:
                    Formula* Parse(std::istream& formula);
            };


            class FOFormula : public Formula
            {
                public:
                    void Interpret(Graph &G, std::list<std::string> parameters);
                    virtual bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment) = 0;
            };


			class FO_Parameter
			{
				private:
					FO_Parameter* Next;
					std::string* variablename;
				public:
					FO_Parameter(std::string* variablename, FO_Parameter* Next);
					~FO_Parameter();

					std::string GetVariableName();
					FO_Parameter* GetNext();
			};


            class FO_Atomic : public FOFormula
            {
                protected:
                    std::string* relation;
                    std::list<std::string> parameters;
                public:
                    FO_Atomic(std::string* relation, FO_Parameter* parameters);
                    ~FO_Atomic();
                    Formula* Clone();

                    bool EdgeMatchesRelation(Edge* e, std::string relation);
                    bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_True : public FOFormula
            {
                public:
                    FO_True();
                    ~FO_True();
                    Formula* Clone();

                    bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_False : public FOFormula
            {
                public:
                    FO_False();
                    ~FO_False();
                    Formula* Clone();

                    bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_Eq : public FOFormula
            {
				private:
					std::string* param1;
					std::string* param2;
				public:
                    FO_Eq(std::string* param1, std::string* param2);
                    ~FO_Eq();
                    Formula* Clone();

                    bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_Neq : public FOFormula
            {
				private:
					std::string* param1;
					std::string* param2;
                public:
                    FO_Neq(std::string* param1, std::string* param2);
                    ~FO_Neq();
                    Formula* Clone();

                    bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_Not : public FOFormula
            {
                protected:
                    FOFormula* phi;
                public:
                    FO_Not(FOFormula* phi);
                    ~FO_Not();
                    Formula* Clone();

                    bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_And : public FOFormula
            {
                protected:
                    FOFormula* phi1;
                    FOFormula* phi2;
                public:
                    FO_And(FOFormula* phi1, FOFormula* phi2);
                    ~FO_And();
                    Formula* Clone();

                    bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_Or : public FOFormula
            {
                protected:
                    FOFormula* phi1;
                    FOFormula* phi2;
                public:
                    FO_Or(FOFormula* phi1, FOFormula* phi2);
                    ~FO_Or();
                    Formula* Clone();

                    bool Interpretation(Graph &G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_Forall : public FOFormula
            {
                protected:
					std::string* variablename;
                    FOFormula* phi;
                public:
                    FO_Forall(std::string* variablename, FOFormula* phi);
                    ~FO_Forall();
                    Formula* Clone();

                    bool Interpretation(Graph& G, std::map<std::string, Vertex*> &VariableAssignment);
            };

            class FO_Exists : public FOFormula
            {
                protected:
					std::string* variablename;
                    FOFormula* phi;
                public:
                    FO_Exists(std::string* variablename, FOFormula* phi);
                    ~FO_Exists();
                    Formula* Clone();

                    bool Interpretation(Graph& G, std::map<std::string, Vertex*> &VariableAssignment);
            };

        } // namespace Logic
    } // namespace OpenGraphtheory

#endif
