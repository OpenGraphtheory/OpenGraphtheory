
#ifndef __OPENGRAPHTHEORY_LOGIC_MODALLOGIC_ML_H
    #define __OPENGRAPHTHEORY_LOGIC_MODALLOGIC_ML_H

    #include "../logic.h"

    namespace OpenGraphtheory
    {
        namespace Logic
        {
            class MLModelChecker : public ModelChecker
            {
                protected:
                    static FactoryRegistrator<ModelChecker> ModelCheckerMLRegistrator;
                public:
                    Formula* Parse(std::istream& formula);
            };


            class MLFormula : public Formula
            {
                public:
                    void Interpret(Graph &G, std::list<std::string> parameters);
                    virtual VertexSet Interpretation(Graph &G) = 0;
            };




            class ML_Atomic : public MLFormula
            {
                protected:
                    std::string* test_description;
                public:
                    ML_Atomic(std::string* atomic_test_description);
                    ~ML_Atomic();
                    Formula* Clone();

                    VertexSet Interpretation(Graph &G);
                    virtual bool Satisfies(Vertex* v);
            };

            class ML_True : public MLFormula
            {
                public:
                    ML_True();
                    ~ML_True();
                    Formula* Clone();

                    VertexSet Interpretation(Graph &G);
            };

            class ML_False : public MLFormula
            {
                public:
                    ML_False();
                    ~ML_False();
                    Formula* Clone();

                    VertexSet Interpretation(Graph &G);
            };

            class ML_Not : public MLFormula
            {
                protected:
                    MLFormula* phi;
                public:
                    ML_Not(MLFormula* phi);
                    ~ML_Not();
                    Formula* Clone();

                    VertexSet Interpretation(Graph &G);
            };

            class ML_And : public MLFormula
            {
                protected:
                    MLFormula* phi1;
                    MLFormula* phi2;
                public:
                    ML_And(MLFormula* phi1, MLFormula* phi2);
                    ~ML_And();
                    Formula* Clone();

                    VertexSet Interpretation(Graph &G);
            };

            class ML_Or : public MLFormula
            {
                protected:
                    MLFormula* phi1;
                    MLFormula* phi2;
                public:
                    ML_Or(MLFormula* phi1, MLFormula* phi2);
                    ~ML_Or();
                    Formula* Clone();

                    VertexSet Interpretation(Graph &G);
            };

            class ML_Diamond : public MLFormula
            {
                protected:
                    MLFormula* phi;
                public:
                    ML_Diamond(MLFormula* phi);
                    ~ML_Diamond();
                    Formula* Clone();

                    VertexSet Interpretation(Graph& G);
            };

            // ====================================================== FORWARDERS

            class ML_Forwarder : public MLFormula
            {
                protected:
                    MLFormula* forwarded;
                public:
                    ML_Forwarder();
                    virtual ~ML_Forwarder();
                    Formula* Clone();

                    VertexSet Interpretation(Graph& G);
            };

            class ML_Box : public ML_Forwarder
            {
                public:
                    ML_Box(MLFormula* phi);
            };

        } // namespace Logic
    } // namespace OpenGraphtheory

#endif
