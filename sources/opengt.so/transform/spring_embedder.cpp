
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

        float width       =  800;
        float height      =  600;

        float c_repel     =    0.02;    // force with which vertices push each other off
        float c_spring    =    2;  // force with which adjacent vertices attract each other
                                     // must not be 0 (or division by zero error will happen)
        float unstressed_spring_length = 100; // if distance < this, then no more force between them
        float delta       =    0.5;  // scaling factor to make the movement more smooth
        float threshold   =    5;    // stop if this no vertex moves more than this far

        int update_interval = 1;
        long usleep_after_update = 250000; //41666; //83333;


        // -----------------------------------------------------------------------------

        float vector_distance(pair<float,float> a, pair<float,float> b)
        {
            return sqrt((b.first-a.first)*(b.first-a.first)+(b.second-a.second)*(b.second-a.second));
        }

        pair<float,float> coulomb(float x1, float y1, float x2, float y2)
        {
            // coulomb's law:
            //                 u-v    c_repel
            // f_repel(u,v) = ----- * -------
            //                |u-v|   |u-v|²

            float distance = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
            if(distance*distance < 0.001)
                return pair<float,float>(0,0);
            distance /= unstressed_spring_length;

            float xe = (x2-x1)/distance;
            float ye = (y2-y1)/distance;
            float factor = 10 * c_repel / (distance*distance);
            return pair<float,float>(factor*xe, factor*ye);
        }

        pair<float,float> hooke(float x1, float y1, float x2, float y2)
        {
            // hooke's law:
            // attraction force of a spring is proportional to distance

            float distance = vector_distance(pair<float,float>(x1,y1), pair<float,float>(x2,y2));
            float xe = (x1-x2)/distance;
            float ye = (y1-y2)/distance;
            // (xe,ye) is normalized vector between the two points

            float factor = 0;
            if(distance > unstressed_spring_length)
                factor = c_spring * (distance/unstressed_spring_length);
            return pair<float,float>(factor*xe, factor*ye);
        }

        pair<float,float> vector_add(pair<float,float> a, pair<float,float> b)
        {
            return pair<float,float>(a.first+b.first, a.second+b.second);
        }

        pair<float,float> vector_subtract(pair<float,float> a, pair<float,float> b)
        {
            return pair<float,float>(a.first-b.first, a.second-b.second);
        }


        // -----------------------------------------------------------------------------


        void SpringEmbed(Graph& G, GraphWindow* display)
        {
            vector<pair<float,float> > tractions;

            // init
            srand ( time(NULL) );
            for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++)
            {
                a.SetX(rand());
                a.SetX(a.GetX() - floor(a.GetX()/width)*width);
                a.SetY(rand());
                a.SetY(a.GetY() - floor(a.GetY()/height)*height);
                // should make sure that no two vertices have the same position
                tractions.push_back(pair<float,float>(0,0));
            }

            float max_movement;
            int updates = 0;
            do
            {
                // compute movement
                max_movement = 0;
                int i = 0;
                for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++, i++)
                {
                    pair<float,float> traction(0,0);

                    // compute forces on a by the other vertices
                    for(Graph::VertexIterator b = G.BeginVertices(); b != G.EndVertices(); b++)
                    {
                        if(b==a)
                            continue;

                        // force on a by vertex b
                        vector_distance(pair<float,float>(a.GetX(), a.GetY()), pair<float,float>(b.GetX(), b.GetY()));
                            traction = vector_subtract(traction, coulomb(a.GetX(), a.GetY(), b.GetX(), b.GetY()));

                        if(a.Adjacent(b))
                            traction = vector_subtract(traction, hooke(a.GetX(), a.GetY(), b.GetX(), b.GetY()));
                    }

                    tractions[i] = traction;
                    // for the loop-condition
                    if(traction.first*traction.first + traction.second*traction.second > max_movement)
                        max_movement = traction.first*traction.first + traction.second*traction.second;
                }

                // execute movement
                Graph::VertexIterator a = G.BeginVertices();
                for(int i = 0; a != G.EndVertices(); a++, i++)
                {
                    a.SetX( max(0.0f, min( width,  a.GetX() + delta * tractions[i].first ) ) );
                    a.SetY( max(0.0f, min( height, a.GetY() + delta * tractions[i].second ) ) );
                }

                if(display != NULL)
                    if(++updates > update_interval)
                    {
                        display->Update();
                        usleep(usleep_after_update);
                        updates = 0;
                    }
            }
            while(max_movement > threshold*threshold);
        }

        void TransformSpringEmbed(Graph& G, list<float> parameters)
        {
            GraphWindow win(width,height,&G);
            win.Update();
            SpringEmbed(G, &win);
            win.WaitUntilClosed();
        }


    }
}
