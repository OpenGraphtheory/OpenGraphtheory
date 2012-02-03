
#include <math.h>
#include "transformation.h"
#include "../../opengt.so/opengt.h"
#include <list>
using namespace std;
using namespace OpenGraphtheory;

namespace OpenGraphtheory
{
    namespace Transform
    {

        Transformer::Transformer()
        {

        }

        Transformer::Transformer(string name, string description, int MinParameterCount, int MaxParameterCount, void TransformerFunction(Graph& G, list<float> parameter))
        {
            this->name = name;
            this->description = description;
            this->MinParamCount = MinParameterCount;
            this->MaxParamCount = MaxParameterCount;
            this->TransformerFunction = TransformerFunction;
        }



        void Transformer::Transform(Graph& G, list<float> parameter)
        {
            if(MinParamCount > 0 && (((int)parameter.size()) < MinParamCount))
            {
                stringstream exception;
                exception << "transformer \"" << name << "\" requires at least " << MinParamCount << " parameters";
                throw exception.str().c_str();
            }
            if(MaxParamCount >= 0 && (((int)parameter.size()) > MaxParamCount))
            {
                stringstream exception;
                exception << "transformer \"" << name << "\" allows at most " << MaxParamCount << " parameters";
                throw exception.str().c_str();
            }
            this->TransformerFunction(G, parameter);
        }



        map<string, Transformer> Transformer::GetTransformers()
        {
            map<string, Transformer> result;

            result["linear"] = Transformer("linear",
                                     "for a linear transformation of the graph",
                                     4, 6,
                                     TransformLinear);
            result["scale"] = Transformer("scale",
                                     "to fit the graph into the rectangle given by parameters (width height )",
                                     2, 2,
                                     TransformScale);
            result["shift"] = Transformer("shift",
                                     "for adding constant values to the coordinates of the vertices",
                                     1, 2,
                                     TransformShift);
            result["rotate"] = Transformer("rotate",
                                     "for rotating the graph (parameter 2 and 3 to set the center of the rotation)",
                                     1, 3,
                                     TransformRotate);
            result["aspectscale"] = Transformer("",
                                     "for a transformation of the graph",
                                     1, 1,
                                     TransformScalePreservingAspect);

            return result;
        }


        void Transformer::Transform(Graph& G, list<float> parameter, string transformer)
        {
            map<string, Transformer> Transformers = GetTransformers();
            if(Transformers.find(transformer) == Transformers.end())
                throw "unknown generator";
            Transformers[transformer].Transform(G, parameter);
        }




        /// ( A11  A12 )     (OldX)   (B1)   (NewX)
        /// ( A21  A22 )  *  (OldY) + (B2) = (NewY)
        void TransformLinear(Graph& G, float A11, float A12, float A21, float A22, float B1, float B2)
        {
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                float OldX = v.GetX();
                float OldY = v.GetY();

                v.SetX( A11 * OldX + A12 * OldY + B1);
                v.SetY( A21 * OldX + A22 * OldY + B2);
            }
        }

        void TransformLinear(Graph& G, list<float> parameters)
        {
            float A11, A12, A21, A22, B1=0, B2=0;
            A11 = parameters.front();
            parameters.pop_front();
            A12 = parameters.front();
            parameters.pop_front();
            A21 = parameters.front();
            parameters.pop_front();
            A22 = parameters.front();
            parameters.pop_front();
            if(parameters.size() > 0)
            {
                B1 = parameters.front();
                parameters.pop_front();
                if(parameters.size() > 0)
                    B2 = parameters.front();
            }

            TransformLinear(G, A11, A12, A21, A22, B1, B2);
        }

        void TransformScale(Graph& G, list<float> parameters)
        {
            float width=parameters.front();
            parameters.pop_front();
            float height=parameters.front();

            Graph::VertexIterator v1 = G.BeginVertices();
            float maxx = v1.GetX(), minx = v1.GetX(), maxy = v1.GetY(), miny = v1.GetY();
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                if(v1.GetX() < minx)
                    minx = v1.GetX();
                if(v1.GetX() > maxx)
                    maxx = v1.GetX();
                if(v1.GetY() < miny)
                    miny = v1.GetY();
                if(v1.GetY() > maxy)
                    maxy = v1.GetY();
            }

            float scaleX = width / (maxx-minx);
            float scaleY = height / (maxy-miny);

            TransformLinear(G, scaleX, 0, 0, scaleY, -scaleX * minx, -scaleY * miny);

            /*
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                v.SetX( scaleX * (v.GetX() - minx));
                v.SetY( scaleY * (v.GetY() - miny));
            }
            */
        }

        void TransformScalePreservingAspect(Graph& G, list<float> parameters)
        {
            float width=parameters.front();
            parameters.pop_front();
            float height=parameters.front();

            Graph::VertexIterator v1 = G.BeginVertices();
            float maxx = v1.GetX(), minx = v1.GetX(), maxy = v1.GetY(), miny = v1.GetY();
            for(v1++; v1 != G.EndVertices(); v1++)
            {
                if(v1.GetX() < minx)
                    minx = v1.GetX();
                if(v1.GetX() > maxx)
                    maxx = v1.GetX();
                if(v1.GetY() < miny)
                    miny = v1.GetY();
                if(v1.GetY() > maxy)
                    maxy = v1.GetY();
            }

            float scaleX = width / (maxx-minx);
            float scaleY = height / (maxy-miny);
            float scale = scaleX < scaleY ? scaleX : scaleY;

            TransformLinear(G, scale, 0, 0, scale, -scale*minx, -scale*miny);

            /*
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                v.SetX( scale * (v.GetX() - minx));
                v.SetY( scale * (v.GetY() - miny));
            }
            */
        }


        void TransformShift(Graph& G, list<float> parameters)
        {
            float x = parameters.front();
            parameters.pop_front();
            float y = 0;
            if(parameters.size() > 0)
                y = parameters.front();

            TransformLinear(G, 1, 0, 0, 1, x, y);
        }


        //    x' = x*cos a + y*sin a,
        //    y' = − x*sin a + y*cos a,
        // additional parameters define the center of the rotation
        void TransformRotate(Graph& G, list<float> parameters)
        {
            float angle=parameters.front();
            float xshift = 0;
            float yshift = 0;

            if(parameters.size() == 3)
            {
                parameters.pop_front();
                xshift = parameters.front();
                parameters.pop_front();
                yshift = parameters.front();
                TransformLinear(G, 1, 0, 0, 1, -xshift, -yshift);
            }

            TransformLinear(G, cos(angle), sin(angle), -sin(angle), cos(angle), xshift, yshift);
        }
    }
}
