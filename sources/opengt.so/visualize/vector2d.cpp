
#include<math.h>
#include "vector2d.h"

namespace OpenGraphtheory
{
    namespace Visualization
    {

        Vector2D::Vector2D(float X, float Y)
        {
            x = X;
            y = Y;
        }

        Vector2D::Vector2D(const Vector2D& Copy)
        {
            x = Copy.x;
            y = Copy.y;
        }
        bool Vector2D::operator=(const Vector2D &Copy)
        {
            return (x = Copy.x) && (y = Copy.y);
        }


        Vector2D Vector2D::operator+(const Vector2D& Sum) const
        {
            return Vector2D(x+Sum.x, y+Sum.y);
        }

        Vector2D Vector2D::operator-(const Vector2D& Diff) const
        {
            return Vector2D(x-Diff.x, y-Diff.y);
        }

        float Vector2D::operator*(const Vector2D& Prod) const
        {
            return x*Prod.x + y*Prod.y;
        }

        Vector2D Vector2D::operator*(float Prod) const
        {
            return Vector2D(x*Prod,y*Prod);
        }

        Vector2D Vector2D::operator/(float Div) const
        {
            return Vector2D(x/Div, y/Div);
        }


        Vector2D Vector2D::Perpendicular() const
        {
            return Vector2D(y, -x);
        }

        float Vector2D::Length() const
        {
            return sqrt(x*x+y*y);
        }

        Vector2D Vector2D::Normalized() const
        {
            return *this / Length();
        }

        float Vector2D::Theta() const
        {
            float alpha = asin( Normalized().y );
            alpha *= 180 / M_PI; // radian to degrees

            if(x > 0)
            {
                if(y > 0)
                    return(alpha);
                else
                    return(360 + alpha); // alpha is negative
            }
            else
            {
                return(180 - alpha); // alpha is negative
            }
        }


        float Vector2D::Distance(const Vector2D& a, const Vector2D& b)
        {
            return (a-b).Length();
        }

        Vector2D Vector2D::Intersection(const Vector2D& A, const Vector2D& X, const Vector2D& C, const Vector2D& Y)
        {
            Vector2D B = X-A;
            Vector2D D = Y-C;

            float k = ((D.x*C.y - D.y*C.x) + (A.x*D.y - A.y*D.x)) / (D.x*B.y - D.y*B.x);

            return A + B*k;
        }

    }
}
