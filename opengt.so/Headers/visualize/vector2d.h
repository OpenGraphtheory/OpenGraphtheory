#ifndef __OPENGRAPHTHEORY_VISUALIZE_VECTOR2D_H
    #define __OPENGRAPHTHEORY_VISUALIZE_VECTOR2D_H

    #include<math.h>

    namespace OpenGraphtheory
    {
        namespace Visualization
        {

            class Vector2D
            {
                public:
                    float x;
                    float y;
                    Vector2D(float X, float Y);
                    Vector2D(const Vector2D& Copy); // Copy Constructor
                    Vector2D operator=(const Vector2D &Copy);

                    Vector2D operator+(const Vector2D& Sum) const;
                    Vector2D operator-(const Vector2D& Diff) const;
                    void operator+=(const Vector2D& Sum);
                    void operator-=(const Vector2D& Diff);

                    float operator*(const Vector2D& Prod) const; // Euclidian Product
                    Vector2D operator*(float Prod) const;
                    void operator*=(float Prod);
                    Vector2D operator/(float Div) const;
                    void operator/=(float Div);

                    Vector2D Perpendicular() const;
                    float Length() const;
                    Vector2D Normalized() const;
                    float Theta() const; // angle between the vector and the x-axis (in degrees)

                    static float Distance(const Vector2D& a, const Vector2D& b);
                    static Vector2D Intersection(const Vector2D& a,const Vector2D& b,const Vector2D& c,const Vector2D& d);
            };

        }
    }

#endif

