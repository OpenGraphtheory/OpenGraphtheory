
#include<vector>
using namespace std;

namespace OpenGraphtheory
{
    namespace Visualization
    {

        class VectorND
        {
            public:
                vector<float> vec;
                VectorND(int size);
                VectorND(const vector<float> vect);
                VectorND(const VectorND& Copy); // Copy Constructor
                VectorND& operator=(const VectorND &Copy);
                float operator[](unsigned int index);

                VectorND operator+(const VectorND& Sum) const;
                VectorND operator-(const VectorND& Diff) const;
                void operator+=(const VectorND& Sum);
                void operator-=(const VectorND& Diff);

                float operator*(const VectorND& Prod) const; // Euclidian Product
                VectorND operator*(float Prod) const;
                VectorND operator/(float Div) const;

                float Length() const;
                VectorND Normalized() const;
                //float Theta() const; // angle between the vector and the x-axis

                static float Distance(const VectorND& a, const VectorND& b);
        };

    }
}



