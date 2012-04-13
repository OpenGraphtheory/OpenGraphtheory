
#include <list>
#include "../opengt.h"
using namespace std;

namespace OpenGraphtheory
{
    namespace Transform
    {

        class Transformer
        {
            public:
                string name;
                string description;
                int MinParamCount;
                int MaxParamCount;
            private:
                void (*TransformerFunction)(Graph& G, vector<float> parameter);

            public:
                static map<string, Transformer> GetTransformers();
                Transformer();
                Transformer(string name, string description, int MinParamCount, int MaxParamCount, void TransformerFunction(Graph& G, vector<float> parameter));

                void Transform(Graph& G, vector<float> parameter);
                static void Transform(Graph& G, vector<float> parameter, string generator);
        };


        void TransformRotate(Graph& G, vector<float> parameter);
        void TransformShift(Graph& G, vector<float> parameter);
        void TransformScale(Graph& G, vector<float> parameter);
        void TransformSpringEmbed(Graph& G, vector<float> parameter);
        void TransformSpringEmbed3D(Graph& G, vector<float> parameter);
        void TransformScalePreservingAspect(Graph& G, vector<float> parameter);

        void TransformLinear(Graph& G, vector<float> parameter);
        void TransformLinear(Graph& G, float A11, float A12, float A21, float A22, float B1=0, float B2=0);

    }
}

