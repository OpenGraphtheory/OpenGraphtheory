
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
                    virtual std::set<Graph::VertexIterator> Interpretation(Graph &G) = 0;
            };




            class ML_Atomic : public MLFormula
            {
                protected:
                    std::string* test_description;
                public:
                    ML_Atomic(std::string* atomic_test_description);
                    ~ML_Atomic();
                    Formula* Clone();

                    std::set<Graph::VertexIterator> Interpretation(Graph &G);
                    virtual bool Satisfies(Graph::VertexIterator v);
            };

            class ML_True : public MLFormula
            {
                public:
                    ML_True();
                    ~ML_True();
                    Formula* Clone();

                    std::set<Graph::VertexIterator> Interpretation(Graph &G);
            };

            class ML_False : public MLFormula
            {
                public:
                    ML_False();
                    ~ML_False();
                    Formula* Clone();

                    std::set<Graph::VertexIterator> Interpretation(Graph &G);
            };

            class ML_Not : public MLFormula
            {
                protected:
                    MLFormula* phi;
                public:
                    ML_Not(MLFormula* phi);
                    ~ML_Not();
                    Formula* Clone();

                    std::set<Graph::VertexIterator> Interpretation(Graph &G);
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

                    std::set<Graph::VertexIterator> Interpretation(Graph &G);
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

                    std::set<Graph::VertexIterator> Interpretation(Graph &G);
            };

            class ML_Box : public MLFormula
            {
                protected:
                    MLFormula* phi;
                public:
                    ML_Box(MLFormula* phi);
                    ~ML_Box();
                    Formula* Clone();

                    std::set<Graph::VertexIterator> Interpretation(Graph& G);
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

                    std::set<Graph::VertexIterator> Interpretation(Graph& G);
            };

            class ML_Diamond : public ML_Forwarder
            {
                public:
                    ML_Diamond(MLFormula* phi);
            };

        } // namespace Logic
    } // namespace OpenGraphtheory

#endif
