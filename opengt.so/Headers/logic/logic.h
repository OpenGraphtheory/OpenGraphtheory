
#ifndef __OPENGT_LOGIC_LOGIC_H
    #define __OPENGT_LOGIC_LOGIC_H

    #include "../factory.h"
    #include "../opengt.h"
    #include<istream>
    #include<string>

    namespace OpenGraphtheory
    {
        namespace Logic
        {

            class Formula
            {
                public:
                    virtual void Interpret(Graph &G) = 0;
            };

            class ModelChecker
            {
                protected:
                    virtual Formula* Parse(std::istream& is) = 0;
                public:
                    static Factory<ModelChecker> ModelCheckerFactory;
                    virtual ~ModelChecker();
                    
                    void ModelCheck(Graph& G, std::istream& formula);
            };

        }
    }

#endif
 
