
#ifndef __OPENGT_LOGIC_COMPUTATIONTREELOGIC_CTL_H
    #define __OPENGT_LOGIC_COMPUTATIONTREELOGIC_CTL_H

    #include "../logic.h"

    namespace OpenGraphtheory
    {
        namespace Logic
        {
            class CTLModelChecker : public ModelChecker
            {
                protected:
                    static FactoryRegistrator<ModelChecker> ModelCheckerCTLRegistrator;
                public:
                    Formula* Parse(std::istream& formula);
            };


            class CTLFormula : public Formula
            {
                public:
                    void Interpret(Graph &G);
                    virtual set<Graph::VertexIterator> Interpretation(Graph &G) = 0;
            };

            class CTL_Atomic : public CTLFormula
            {
                protected:
                    std::string* test_description;
                public:
                    CTL_Atomic(std::string* atomic_test_description);
                    ~CTL_Atomic();
                    set<Graph::VertexIterator> Interpretation(Graph &G);
                    virtual bool Satisfies(Graph::VertexIterator v);
            };

            class CTL_True : public CTLFormula
            {
                public:
                    CTL_True();
                    ~CTL_True();
                    set<Graph::VertexIterator> Interpretation(Graph &G);
            };

            class CTL_False : public CTLFormula
            {
                public:
                    CTL_False();
                    ~CTL_False();
                    set<Graph::VertexIterator> Interpretation(Graph &G);
            };

            class CTL_Not : public CTLFormula
            {
                protected:
                    CTLFormula* phi;
                public:
                    CTL_Not(CTLFormula* phi);
                    ~CTL_Not();
                    set<Graph::VertexIterator> Interpretation(Graph &G);
            };

            class CTL_And : public CTLFormula
            {
                protected:
                    CTLFormula* phi1;
                    CTLFormula* phi2;
                public:
                    CTL_And(CTLFormula* phi1, CTLFormula* phi2);
                    ~CTL_And();
                    set<Graph::VertexIterator> Interpretation(Graph &G);
            };

            class CTL_Or : public CTLFormula
            {
                protected:
                    CTLFormula* phi1;
                    CTLFormula* phi2;
                public:
                    CTL_Or(CTLFormula* phi1, CTLFormula* phi2);
                    ~CTL_Or();
                    set<Graph::VertexIterator> Interpretation(Graph &G);
            };

            class CTL_ExistsNext : public CTLFormula
            {
                protected:
                    CTLFormula* phi;
                public:
                    CTL_ExistsNext(CTLFormula* phi);
                    ~CTL_ExistsNext();
                    set<Graph::VertexIterator> Interpretation(Graph& G);
            };

            class CTL_ExistsGlobally : public CTLFormula
            {
                protected:
                    CTLFormula* phi;
                public:
                    CTL_ExistsGlobally(CTLFormula* phi);
                    ~CTL_ExistsGlobally();
                    set<Graph::VertexIterator> Interpretation(Graph& G);
            };

            class CTL_ExistsUntil : public CTLFormula
            {
                protected:
                    CTLFormula* phi1;
                    CTLFormula* phi2;
                public:
                    CTL_ExistsUntil(CTLFormula* phi1, CTLFormula* phi2);
                    ~CTL_ExistsUntil();
                    set<Graph::VertexIterator> Interpretation(Graph& G);
            };


            // ====================================================== FORWARDERS

            class CTL_Forwarder : public CTLFormula
            {
                protected:
                    CTLFormula* forwarded;
                public:
                    virtual ~CTL_Forwarder();
                    set<Graph::VertexIterator> Interpretation(Graph& G);
            };


            class CTL_ExistsFinally : public CTL_Forwarder
            {
                public:
                    CTL_ExistsFinally(CTLFormula* phi);
            };

            class CTL_AlwaysNext : public CTL_Forwarder
            {
                public:
                    CTL_AlwaysNext(CTLFormula* phi);
            };

            class CTL_AlwaysGlobally : public CTL_Forwarder
            {
                public:
                    CTL_AlwaysGlobally(CTLFormula* phi);
            };

            class CTL_AlwaysFinally : public CTL_Forwarder
            {
                public:
                    CTL_AlwaysFinally(CTLFormula* phi);
            };

            /*
            class CTL_AlwaysUntil : public CTL_Forwarder
            {
                public:
                    CTL_AlwaysUntil(CTLFormula* phi1, CTL*Formula* phi2);
            };
            */

        } // namespace Logic
    } // namespace OpenGraphtheory

#endif