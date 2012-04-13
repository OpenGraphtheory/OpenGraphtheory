
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

        Transformer::Transformer(string name, string description, int MinParameterCount, int MaxParameterCount, void TransformerFunction(Graph& G, vector<float> parameter))
        {
            this->name = name;
            this->description = description;
            this->MinParamCount = MinParameterCount;
            this->MaxParamCount = MaxParameterCount;
            this->TransformerFunction = TransformerFunction;
        }



        void Transformer::Transform(Graph& G, vector<float> parameter)
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
                                     "to fit the graph into the rectangle given by parameters (width height [depth ...])",
                                     2, -1,
                                     TransformScale);
            result["shift"] = Transformer("shift",
                                     "for adding constant values to the coordinates of the vertices",
                                     1, -1,
                                     TransformShift);
            result["rotate"] = Transformer("rotate",
                                     "for rotating the graph (parameter 2 and 3 to set the center of the rotation)",
                                     1, 3,
                                     TransformRotate);
            result["springembed"] = Transformer("springembed",
                                     "assigns coordinates to vertices, using the spring-embedding technique",
                                     0,0,
                                     TransformSpringEmbed);
            result["springembed3d"] = Transformer("springembed3d",
                                     "assigns coordinates to vertices, using the spring-embedding technique in a 3-dimensional space",
                                     0,0,
                                     TransformSpringEmbed3D);
            result["aspectscale"] = Transformer("",
                                     "for a transformation of the graph",
                                     2, -1,
                                     TransformScalePreservingAspect);

            return result;
        }


        void Transformer::Transform(Graph& G, vector<float> parameter, string transformer)
        {
            map<string, Transformer> Transformers = GetTransformers();
            if(Transformers.find(transformer) == Transformers.end())
                throw "unknown transformer";
            Transformers[transformer].Transform(G, parameter);
        }




        /// A * OldCoordinates + B = NewCoordinates
        void TransformLinear(Graph& G, vector<vector<float> > A, vector<float> B)
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

        void TransformLinear(Graph& G, vector<float> parameters)
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

        void TransformScale(Graph& G, vector<float> parameters)
        {
            unsigned int dimensions = 0;
            vector<float> dimension_min;
            vector<float> dimension_max;
            vector<float> coordinates;

            // find minimum and maximum values on all dimensions
            for(Graph::VertexIterator v1 = G.BeginVertices(); v1 != G.EndVertices(); v1++)
            {
                coordinates = v1.GetCoordinates();
                for(int j = min(dimensions, coordinates.size())-1; j >= 0; --j)
                {
                    if(coordinates[j] < dimension_min[j])
                        dimension_min[j] = coordinates[j];
                    if(coordinates[j] > dimension_max[j])
                        dimension_max[j] = coordinates[j];
                }

                // add more dimensions, if neccessary
                if(coordinates.size() > dimensions)
                {
                    for(unsigned int j = dimensions; j < coordinates.size(); j++)
                    {
                        dimension_min.push_back(coordinates[j]);
                        dimension_max.push_back(coordinates[j]);
                    }
                    dimensions = coordinates.size();
                }
            }

            // set scales for all dimensions
            vector<float> scale(dimensions);
            for(unsigned int i = dimensions-1; i >= 0; --i)
                if(dimension_max[i] > dimension_min[i])
                    scale[i] = parameters[i] / (dimension_max[i] - dimension_min[i]);
                else
                    scale[i] = 0.0f;

            // scale
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                for(unsigned int i = coordinates.size()-1; i >= 0; --i)
                    coordinates[i] = (coordinates[i] - dimension_min[i]) * scale[i];
                v.SetCoordinates(coordinates);
            }
        }

        void TransformScalePreservingAspect(Graph& G, vector<float> parameters)
        {
            unsigned int dimensions = 0;
            vector<float> dimension_min;
            vector<float> dimension_max;
            vector<float> coordinates;

            // find minimum and maximum values on all dimensions
            for(Graph::VertexIterator v1 = G.BeginVertices(); v1 != G.EndVertices(); v1++)
            {
                coordinates = v1.GetCoordinates();
                for(int j = min(dimensions, coordinates.size())-1; j >= 0; --j)
                {
                    if(coordinates[j] < dimension_min[j])
                        dimension_min[j] = coordinates[j];
                    if(coordinates[j] > dimension_max[j])
                        dimension_max[j] = coordinates[j];
                }

                // add more dimensions, if neccessary
                if(coordinates.size() > dimensions)
                {
                    for(unsigned int j = dimensions; j < coordinates.size(); j++)
                    {
                        dimension_min.push_back(coordinates[j]);
                        dimension_max.push_back(coordinates[j]);
                    }
                    dimensions = coordinates.size();
                }
            }

            // select minimum of scales
            float scale = parameters[0] / (dimension_max[0] - dimension_min[0]);
            for(unsigned int i = dimensions-1; i >= 1; --i)
                if(dimension_max[i] > dimension_min[i])
                    if(parameters[i] / (dimension_max[i] - dimension_min[i]) < scale)
                        scale = parameters[i] / (dimension_max[i] - dimension_min[i]);

            // scale
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                for(unsigned int i = coordinates.size()-1; i >= 0; --i)
                    coordinates[i] = (coordinates[i] - dimension_min[i]) * scale;
                v.SetCoordinates(coordinates);
            }
        }


        void TransformShift(Graph& G, vector<float> parameters)
        {
            for(Graph::VertexIterator v = G.BeginVertices(); v != G.EndVertices(); v++)
            {
                vector<float> coordinates = v.GetCoordinates();
                for(unsigned int i = min(coordinates.size(), parameters.size())-1; i >= 0; --i)
                    coordinates[i] += parameters[i];
                v.SetCoordinates(coordinates);
            }
        }


        //    x' = x*cos a + y*sin a,
        //    y' = − x*sin a + y*cos a,
        // additional parameters define the center of the rotation
        void TransformRotate(Graph& G, vector<float> parameters)
        {
            float angle=parameters[0];
            float xshift = 0;
            float yshift = 0;

            if(parameters.size() == 3)
            {
                xshift = parameters[1];
                yshift = parameters[2];
                TransformLinear(G, 1, 0, 0, 1, -xshift, -yshift);
            }

            TransformLinear(G, cos(angle), sin(angle), -sin(angle), cos(angle), xshift, yshift);
        }
    }
}
