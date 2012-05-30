
#include "../../Headers/transform/linear.h"

using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Transform
    {

        //FactoryRegistrator<Transformer> TransformerLINEAR::TransformerLinearRegistrator(
        //    &Transformer::TransformerFactory, "linear", new DefaultInstantiator<Transformer, TransformerLINEAR>(
        //        "linear", "linear transforming", "http://en.wikipedia.org/wiki/Linear_map"));

        TransformerLINEAR::TransformerLINEAR()
        {
            MinParamCount = 1;
            MaxParamCount = -1;
        }


        /// A * OldCoordinates + B = NewCoordinates
        void TransformerLINEAR::TransformLinear(Graph& G, vector<vector<float> > A, vector<float> B)
        {
            while(B.size() < A.size())
                B.push_back(0.0f);
            while(A.size() < B.size())
                A.push_back(vector<float>(0));

            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> OldCoordinates = v.GetCoordinates();
                vector<float> NewCoordinates = B;

                for(unsigned int y = A.size()-1; y >= 0; --y)
                    for(unsigned int x = min(OldCoordinates.size(), A[y].size())-1; x >= 0; --x)
                        NewCoordinates[y] += OldCoordinates[x] * A[y][x];

                v.SetCoordinates(NewCoordinates);
            }
        }

        void TransformerLINEAR::TransformLinear(Graph& G, float A11, float A12, float A21, float A22, float B1, float B2)
        {
            vector<float> B;
            B.push_back(B1);
            B.push_back(B2);

            vector<float> A1;
            A1.push_back(A11);
            A1.push_back(A12);

            vector<float> A2;
            A2.push_back(A21);
            A2.push_back(A22);

            vector<vector<float> > A;
            A.push_back(A1);
            A.push_back(A2);

            TransformLinear(G, A, B);
        }


        void TransformerLINEAR::DoTransform(Graph& G, vector<float> parameters)
        {
            float A11, A12, A21, A22, B1=0.0f, B2=0.0f;
            A11 = parameters[0];
            A12 = parameters[1];
            A21 = parameters[2];
            A22 = parameters[3];
            if(parameters.size() > 4)
            {
                B1 = parameters[4];
                if(parameters.size() > 5)
                    B2 = parameters[5];
            }

            TransformLinear(G, A11, A12, A21, A22, B1, B2);
        }

    }
}
