
#include <iostream>
#include <math.h>
#include <stdlib.h> // for random numbers
#include "../opengt.h"
#include "../visualize/GraphWindow.h"
#include "../visualize/vector3d.h"
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

        Vector3D coulomb(Vector3D u, Vector3D v)
        {
            // coulomb's law:
            //                 u-v    c_repel
            // f_repel(u,v) = ----- * -------
            //                |u-v|   |u-v|²

            float distance = (u-v).Length() / unstressed_spring_length;
            if(distance < 0.00001)
                return u * -1;
            Vector3D e = (u-v).Normalized();

            float factor = c_repel / (distance*distance);
            return e * factor;
        }

        Vector3D hooke(Vector3D u, Vector3D v)
        {
            // hooke's law:
            // attraction force of a spring is proportional to distance

            float distance = (u-v).Length() / unstressed_spring_length;

            Vector3D e = (u-v).Normalized();
            // e is normalized vector between the two points

            float factor = distance > 1 ? -c_spring * distance : 0;
            return e * factor;
        }


        // -----------------------------------------------------------------------------


        void SpringEmbed3D(Graph& G, GraphWindow* display)
        {
            vector<Vector3D> tractions;
            unstressed_spring_length = sqrt(width * height / G.NumberOfVertices()) / 2;

            // init
            srand ( time(NULL) );
            for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++)
            {
                a.SetX(rand());
                a.SetX(a.GetX() - floor(a.GetX()/width)*width);
                a.SetY(rand());
                a.SetY(a.GetY() - floor(a.GetY()/height)*height);
                a.SetZ(rand());
                a.SetZ(a.GetZ() - floor(a.GetZ()/depth)*depth);

                // should make sure that no two vertices have the same position
                tractions.push_back(Vector3D(0,0,0));
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
                    Vector3D traction = tractions[i] * friction;

                    // compute forces on a by the other vertices
                    for(Graph::VertexIterator b = G.BeginVertices(); b != G.EndVertices(); b++)
                    {
                        if(b==a)
                            continue;

                        // force on a by vertex b
                        traction += coulomb(Vector3D(a.GetX(), a.GetY(), a.GetZ()), Vector3D(b.GetX(), b.GetY(), b.GetZ()));
                        if(a.Adjacent(b))
                            traction += hooke(Vector3D(a.GetX(), a.GetY(), a.GetZ()), Vector3D(b.GetX(), b.GetY(), b.GetZ()));
                    }

                    tractions[i] = traction;
                }

                // execute movement
                Graph::VertexIterator a = G.BeginVertices();
                for(int i = 0; a != G.EndVertices(); a++, i++)
                {
                    float NewX = max(0.0f, min( width,  a.GetX() + delta * tractions[i].x ) );
                    float NewY = max(0.0f, min( height, a.GetY() + delta * tractions[i].y ) );
                    float NewZ = max(0.0f, min( depth, a.GetZ() + delta * tractions[i].z ) );

                    // for the loop-condition
                    max_movement = max(max_movement, (a.GetX()-NewX)*(a.GetX()-NewX) + (a.GetY()-NewY)*(a.GetY()-NewY) + (a.GetZ()-NewZ)*(a.GetZ()-NewZ));

                    a.SetX( NewX );
                    a.SetY( NewY );
                    a.SetZ( NewZ );
                }

                if(display != NULL)
                    display->Update();

                if(++iteration > nextincrease)
                {
                    movement_threshold++;
                    nextincrease += nextincrease / 3;
                }
            }
            while((max_movement > movement_threshold*unstressed_spring_length));

        }

        void TransformSpringEmbed3D(Graph& G, list<float> parameters)
        {
            GraphWindow win(width,height,&G);
            win.Update();
            SpringEmbed3D(G, &win);
            //win.WaitUntilClosed();
        }


    }
}
