
#include "../../Headers/transform/springembedder.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;

namespace OpenGraphtheory
{
    namespace Transform
    {

        //FactoryRegistrator<Transformer> TransformerSPRINGEMBEDDER::TransformerSpringEmbedderRegistrator(
        //    &Transformer::TransformerFactory, "springembed", new DefaultInstantiator<Transformer, TransformerSPRINGEMBEDDER>(
        //        "springembed", "assigns coordinates to vertices, using the spring-embedding technique",
        //        "http://i11www.iti.uni-karlsruhe.de/_media/teaching/sommer2004/networkdrawing/spring.pdf"));


        TransformerSPRINGEMBEDDER::TransformerSPRINGEMBEDDER()
        {
            c_repel     =    25;
            c_spring    =    20;
            friction    =    0.5;
            unstressed_spring_length = 100;
            delta = 0.2;
            movement_threshold = 0.5;
        }

        // -----------------------------------------------------------------------------

        VectorND TransformerSPRINGEMBEDDER::coulomb(VectorND u, VectorND v)
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

        VectorND TransformerSPRINGEMBEDDER::hooke(VectorND u, VectorND v)
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


        void TransformerSPRINGEMBEDDER::SpringEmbed(Graph& G, GraphWindow* display, int dimensions, vector<float> dimension_limits)
        {
            int width = dimension_limits[0];
            int height = dimension_limits[1];

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

        void TransformerSPRINGEMBEDDER::DoTransform3D(Graph& G, vector<float> parameters)
        {
            int width = 800;
            int height = 600;
            int depth = 600;

            if(parameters.size() > 1)
            {
                width = (int)(parameters[0]+0.5);
                height = (int)(parameters[1]+0.5);
            }
            if(parameters.size() > 2)
                depth = (int)(parameters[2]+0.5);

            GraphWindow win(width,height,&G);
            win.Update();
            vector<float> dimension_limits;
            dimension_limits.push_back(width);
            dimension_limits.push_back(height);
            dimension_limits.push_back(depth);
            SpringEmbed(G, &win, 3, dimension_limits);
            //win.WaitUntilClosed();
        }

        void TransformerSPRINGEMBEDDER::DoTransform(Graph& G, vector<float> parameters)
        {
            int width = 800;
            int height = 600;

            if(parameters.size() > 1)
            {
                width = (int)(parameters[0]+0.5);
                height = (int)(parameters[1]+0.5);
            }

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
