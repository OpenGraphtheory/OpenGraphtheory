
#include<math.h>
#include "../../Headers/visualize/vectornd.h"

namespace OpenGraphtheory
{
    namespace Visualization
    {

        VectorND::VectorND(vector<float> vect)
        {
            vec = vect;
        }

        VectorND::VectorND(const VectorND& Copy)
        {
            vec = Copy.vec;
        }
        VectorND::VectorND(int size)
        {
            vec = vector<float>(size);
            for(int i = 0; i < size; i++)
                vec[i] = 0.0f;
        }

        VectorND& VectorND::operator=(const VectorND &Copy)
        {
            vec = Copy.vec;
            return *this;
        }
        float VectorND::operator[](unsigned int index)
        {
            return vec[index];
        }


        VectorND VectorND::operator+(const VectorND& Sum) const
        {
            vector<float> result(max(vec.size(), Sum.vec.size()));
            for(unsigned int i = 0; i < vec.size(); i++)
                result[i] = vec[i];
            for(unsigned int i = vec.size(); i < result.size(); i++)
                result[i] = 0.0f;
            for(unsigned int i = 0; i < Sum.vec.size(); i++)
                result[i] += Sum.vec[i];

            return VectorND(result);
        }

        VectorND VectorND::operator-(const VectorND& Diff) const
        {
            vector<float> result(max(vec.size(), Diff.vec.size()));
            for(unsigned int i = 0; i < vec.size(); i++)
                result[i] = vec[i];
            for(unsigned int i = vec.size(); i < result.size(); i++)
                result[i] = 0.0f;
            for(unsigned int i = 0; i < Diff.vec.size(); i++)
                result[i] -= Diff.vec[i];

            return VectorND(result);
        }

        void VectorND::operator+=(const VectorND& Sum)
        {
            operator=(operator+(Sum));
        }

        void VectorND::operator-=(const VectorND& Diff)
        {
            operator=(operator-(Diff));
        }


        float VectorND::operator*(const VectorND& Prod) const
        {
            float sum = 0;
            for(unsigned int i = min(vec.size(), Prod.vec.size())-1; i >= 0; --i)
                sum += vec[i] * Prod.vec[i];
            return sum;
        }

        VectorND VectorND::operator*(float Prod) const
        {
            vector<float> result(vec.size());
            for(unsigned int i = vec.size() -1; i >= 0; --i)
                result[i] = vec[i] * Prod;
            return VectorND(result);
        }

        VectorND VectorND::operator/(float Div) const
        {
            vector<float> result(vec.size());
            for(unsigned int i = vec.size() -1; i >= 0; --i)
                result[i] = vec[i] / Div;
            return VectorND(result);
        }

        float VectorND::Length() const
        {
            float sum = 0;
            for(unsigned int i = vec.size()-1; i >= 0; --i)
                sum += vec[i]*vec[i];
            return sqrt(sum);
        }

        VectorND VectorND::Normalized() const
        {
            return operator/(Length());
        }


        float VectorND::Distance(const VectorND& a, const VectorND& b)
        {
            return (a-b).Length();
        }

    }
}
