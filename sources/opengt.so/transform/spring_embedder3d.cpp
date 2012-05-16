
#include <iostream>
#include <math.h>
#include <stdlib.h> // for random numbers
#include "../opengt.h"
#include "../visualize/GraphWindow.h"
#include "../visualize/vectornd.h"
using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Transform
    {

//        float width       =   800;
//        float height      =   600;
        float depth       =   600;
//        int maxiterations =  1500;

//        float c_repel     =    25; // force with which vertices push each other off
//        float c_spring    =    20;  // force with which adjacent vertices attract each other
                                   // must not be 0 (or division by zero error will happen)
//        float friction    =    0.5;

//        float unstressed_spring_length = 100; // if distance < this, then no more force between them
//        float delta       =    0.2;  // scaling factor to make the movement more smooth
//        float movement_threshold   =    0.5;    // stop if no vertex moves more than this far

        extern float width;
        extern float height;
        extern int iteration;
        extern int nextincrease;
        extern float c_repel;
        extern float c_spring;
        extern float friction;
        extern float unstressed_spring_length;
        extern float delta;
        extern float movement_threshold;

        // -----------------------------------------------------------------------------

        VectorND coulomb(VectorND u, VectorND v)
        {
            // coulomb's law:
            //                 u-v    c_repel
            // f_repel(u,v) = ----- * -------
            //                |u-v|   |u-v|²


            float distance = (u-v).Length() / unstressed_spring_length;
            if(distance < 0.00001)
                return u * -1;
            VectorND e = (u-v).Normalized();

            float factor = c_repel / (distance*distance);
            return e * factor;
        }

        VectorND hooke(VectorND u, VectorND v)
        {
            // hooke's law:
            // attraction force of a spring is proportional to distance

            float distance = (u-v).Length() / unstressed_spring_length;

            VectorND e = (u-v).Normalized();
            // e is normalized vector between the two points

            float factor = distance > 1 ? -c_spring * distance : 0;
            return e * factor;
        }


        // -----------------------------------------------------------------------------


        void SpringEmbed(Graph& G, GraphWindow* display, int dimensions, vector<float> dimension_limits)
        {
            vector<VectorND> tractions;
            unstressed_spring_length = sqrt(width * height / G.NumberOfVertices()) / 2;

            // init
            srand ( time(NULL) );
            for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++)
            {
                vector<float> coordinates;
                for(int i = 0; i < dimensions; i++)
                    coordinates.push_back(fmod(rand(), dimension_limits[i]));

                // should make sure that no two vertices have the same position
                tractions.push_back(VectorND(dimensions));
            }

            float max_movement;
            iteration = 0;
            do
            {
                // compute movement
                max_movement = 0;
                int i = 0;
                for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++, i++)
                {
                    VectorND traction = tractions[i] * friction;

                    // compute forces on a by the other vertices
                    for(Graph::VertexIterator b = G.BeginVertices(); b != G.EndVertices(); b++)
                    {
                        if(b==a)
                            continue;

                        // force on a by vertex b
                        traction += coulomb(VectorND(a.GetCoordinates()), VectorND(b.GetCoordinates()));
                        if(a.Adjacent(b))
                            traction += hooke(VectorND(a.GetCoordinates()), VectorND(b.GetCoordinates()));
                    }

                    tractions[i] = traction;
                }

                // execute movement
                Graph::VertexIterator a = G.BeginVertices();
                for(int i = 0; a != G.EndVertices(); a++, i++)
                {
                    vector<float> OldCoordinates = a.GetCoordinates();
                    vector<float> NewCoordinates(dimensions);
                    for(int j = 0; j < dimensions; j++)
                        NewCoordinates[j] = max(0.0f,min(dimension_limits[j],   OldCoordinates[j] + delta * tractions[i][j]   ));
                    a.SetCoordinates( NewCoordinates );

                    // for the loop-condition
                    float current_movement = 0.0f;
                    for(int j = 0; j < dimensions; j++)
                        current_movement += (OldCoordinates[j] - NewCoordinates[j]) * (OldCoordinates[j] - NewCoordinates[j]);
                    if(current_movement > max_movement)
                        max_movement = current_movement;
                }

                if(display != NULL)
                    display->Update();

                if(++iteration > nextincrease)
                {
                    movement_threshold++;
                    nextincrease += nextincrease / 2;
                }
            }
            while((max_movement > movement_threshold*unstressed_spring_length));

        }

        void TransformSpringEmbed3D(Graph& G, list<float> parameters)
        {
            GraphWindow win(width,height,&G);
            win.Update();
            vector<float> dimension_limits;
            dimension_limits.push_back(width);
            dimension_limits.push_back(height);
            dimension_limits.push_back(depth);
            SpringEmbed(G, &win, 3, dimension_limits);
            //win.WaitUntilClosed();
        }

        void TransformSpringEmbed(Graph& G, list<float> parameters)
        {
            GraphWindow win(width,height,&G);
            win.Update();
            vector<float> dimension_limits;
            dimension_limits.push_back(width);
            dimension_limits.push_back(height);
            SpringEmbed(G, &win, 2, dimension_limits);
            //win.WaitUntilClosed();
        }

    }
}
