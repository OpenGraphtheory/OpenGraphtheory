
#include <list>
#include "../opengt.h"
#include "../factory.h"
using namespace std;

namespace OpenGraphtheory
{
    namespace Transform
    {

        class Transformer
        {
            public:
                int MinParamCount;
                int MaxParamCount;

            protected:
                virtual void DoTransform(Graph& G, vector<float> parameter) = 0;

            public:
                static Factory<Transformer> TransformerFactory;

                virtual ~Transformer();
                void Transform(Graph& G, vector<float> parameter);
                static void Transform(Graph& G, vector<float> parameter, string transformer);
        };

    }
}

