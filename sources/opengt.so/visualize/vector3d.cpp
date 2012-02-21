
#include<math.h>
#include "vector3d.h"

namespace OpenGraphtheory
{
    namespace Visualization
    {

        Vector3D::Vector3D(float X, float Y, float Z)
        {
            x = X;
            y = Y;
            z = Z;
        }

        Vector3D::Vector3D(const Vector3D& Copy)
        {
            x = Copy.x;
            y = Copy.y;
            z = Copy.z;
        }
        bool Vector3D::operator=(const Vector3D &Copy)
        {
            return (x = Copy.x) && (y = Copy.y) && (z = Copy.z);
        }


        Vector3D Vector3D::operator+(const Vector3D& Sum) const
        {
            return Vector3D(x+Sum.x, y+Sum.y, z+Sum.z);
        }

        Vector3D Vector3D::operator-(const Vector3D& Diff) const
        {
            return Vector3D(x-Diff.x, y-Diff.y, z-Diff.z);
        }

        void Vector3D::operator+=(const Vector3D& Sum)
        {
            x += Sum.x;
            y += Sum.y;
            z += Sum.z;
        }

        void Vector3D::operator-=(const Vector3D& Diff)
        {
            x -= Diff.x;
            y -= Diff.y;
            z -= Diff.z;
        }


        float Vector3D::operator*(const Vector3D& Prod) const
        {
            return x*Prod.x + y*Prod.y + z*Prod.z;
        }

        Vector3D Vector3D::operator*(float Prod) const
        {
            return Vector3D(x*Prod,y*Prod,z*Prod);
        }

        Vector3D Vector3D::operator/(float Div) const
        {
            return Vector3D(x/Div, y/Div, z/Div);
        }

        float Vector3D::Length() const
        {
            return sqrt(x*x + y*y + z*z);
        }

        Vector3D Vector3D::Normalized() const
        {
            return *this / Length();
        }


        float Vector3D::Distance(const Vector3D& a, const Vector3D& b)
        {
            return (a-b).Length();
        }

    }
}
