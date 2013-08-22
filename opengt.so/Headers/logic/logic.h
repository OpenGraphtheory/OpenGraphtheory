
#ifndef __OPENGT_LOGIC_LOGIC_H
    #define __OPENGT_LOGIC_LOGIC_H

    #include "../factory.h"
    #include "../opengt.h"
    #include<istream>
    #include<string>
    #include<list>

    namespace OpenGraphtheory
    {
        namespace Logic
        {

            class Formula
            {
                public:
                    virtual void Interpret(Graph &G, std::list<std::string> parameters) = 0;
                    virtual Formula* Clone() = 0;
                    virtual ~Formula();
            };

            class ModelChecker
            {
                protected:
                    virtual Formula* Parse(std::istream& is) = 0;
                public:
                    static Factory<ModelChecker> ModelCheckerFactory;
                    virtual ~ModelChecker();

                    void ModelCheck(Graph& G, std::istream& formula, std::list<std::string> parameters);
                    void ModelCheck(Graph& G, std::string formula, std::list<std::string> parameters);

            };

        }
    }

#endif

