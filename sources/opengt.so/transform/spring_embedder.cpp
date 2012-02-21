
#include <iostream>
#include <math.h>
#include <stdlib.h> // for random numbers
#include "../opengt.h"
#include "../visualize/GraphWindow.h"
using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Transform
    {

        float width       =   800;
        float height      =   600;

        float movement_threshold   =    0.5;    // stop if no vertex moves more than this far
        int nextincrease  =   300;
        int iteration     =     0;

        float c_repel     =    25; // force with which vertices push each other off
        float c_spring    =    20;  // force with which adjacent vertices attract each other
                                   // must not be 0 (or division by zero error will happen)
        float friction    =    0.5;

        float unstressed_spring_length = 100; // if distance < this, then no more force between them
        float delta       =    0.2;  // scaling factor to make the movement more smooth

        // -----------------------------------------------------------------------------

        Vector2D coulomb(Vector2D u, Vector2D v)
        {
            // coulomb's law:
            //                 u-v    c_repel
            // f_repel(u,v) = ----- * -------
            //                |u-v|   |u-v|²

            float distance = (u-v).Length() / unstressed_spring_length;
            if(distance < 0.00001)
                return u * -1;
            Vector2D e = (u-v).Normalized();

            float factor = c_repel / (distance*distance);
            return e * factor;
        }

        Vector2D hooke(Vector2D u, Vector2D v)
        {
            // hooke's law:
            // attraction force of a spring is proportional to distance

            float distance = (u-v).Length() / unstressed_spring_length;

            Vector2D e = (u-v).Normalized();
            // e is normalized vector between the two points

            float factor = distance > 1 ? -c_spring * distance : 0;
            return e * factor;
        }


        // -----------------------------------------------------------------------------


        void SpringEmbed(Graph& G, GraphWindow* display)
        {
            vector<Vector2D> tractions;
            unstressed_spring_length = sqrt(width * height / G.NumberOfVertices()) / 2;

            // init
            srand ( time(NULL) );
            for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++)
            {
                a.SetX(rand());
                a.SetX(a.GetX() - floor(a.GetX()/width)*width);
                a.SetY(rand());
                a.SetY(a.GetY() - floor(a.GetY()/height)*height);
                // should make sure that no two vertices have the same position
                tractions.push_back(Vector2D(0,0));
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
                    Vector2D traction = tractions[i] * friction;

                    // compute forces on a by the other vertices
                    for(Graph::VertexIterator b = G.BeginVertices(); b != G.EndVertices(); b++)
                    {
                        if(b==a)
                            continue;

                        // force on a by vertex b
                        traction += coulomb(Vector2D(a.GetX(), a.GetY()), Vector2D(b.GetX(), b.GetY()));
                        if(a.Adjacent(b))
                            traction += hooke(Vector2D(a.GetX(), a.GetY()), Vector2D(b.GetX(), b.GetY()));
                    }

                    tractions[i] = traction;
                }

                // execute movement
                Graph::VertexIterator a = G.BeginVertices();
                for(int i = 0; a != G.EndVertices(); a++, i++)
                {
                    float NewX = max(0.0f, min( width,  a.GetX() + delta * tractions[i].x ) );
                    float NewY = max(0.0f, min( height, a.GetY() + delta * tractions[i].y ) );

                    // for the loop-condition
                    max_movement = max(max_movement, (a.GetX()-NewX)*(a.GetX()-NewX) + (a.GetY()-NewY)*(a.GetY()-NewY));

                    a.SetX( NewX );
                    a.SetY( NewY );
                }

                if(display != NULL)
                    display->Update();

                if(++iteration > nextincrease)
                {
                    movement_threshold++;
                    nextincrease += nextincrease/3;
                }
            }
            while((max_movement > movement_threshold*unstressed_spring_length));

        }

        void TransformSpringEmbed(Graph& G, list<float> parameters)
        {
            GraphWindow win(width,height,&G);
            win.Update();
            SpringEmbed(G, &win);
            //win.WaitUntilClosed();
        }


    }
}
