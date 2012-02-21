
namespace OpenGraphtheory
{
    namespace Visualization
    {

        class Vector3D
        {
            public:
                float x;
                float y;
                float z;
                Vector3D(float X, float Y, float Z);
                Vector3D(const Vector3D& Copy); // Copy Constructor
                bool operator=(const Vector3D &Copy);

                Vector3D operator+(const Vector3D& Sum) const;
                Vector3D operator-(const Vector3D& Diff) const;
                void operator+=(const Vector3D& Sum);
                void operator-=(const Vector3D& Diff);

                float operator*(const Vector3D& Prod) const; // Euclidian Product
                Vector3D operator*(float Prod) const;
                Vector3D operator/(float Div) const;

                float Length() const;
                Vector3D Normalized() const;
                //float Theta() const; // angle between the vector and the x-axis

                static float Distance(const Vector3D& a, const Vector3D& b);
        };

    }
}



