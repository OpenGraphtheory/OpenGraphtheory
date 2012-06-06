
#include "../../Headers/transform/springembedder.h"

using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Visualization;
using namespace OpenGraphtheory::IntermediateSteps;

namespace OpenGraphtheory
{
    namespace Transform
    {

        FactoryRegistrator<Transformer> TransformerSPRINGEMBEDDER::TransformerSpringEmbedderRegistrator(
            &Transformer::TransformerFactory, "springembed", new DefaultInstantiator<Transformer, TransformerSPRINGEMBEDDER>(
                "springembed", "assigns coordinates to vertices, using the spring-embedding technique",
                "http://i11www.iti.uni-karlsruhe.de/_media/teaching/sommer2004/networkdrawing/spring.pdf"));

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

        TransformerSPRINGEMBEDDER::TransformerSPRINGEMBEDDER()
        {
            MinParamCount = 0;
            MaxParamCount = -1;

            c_repel     =    25;
            c_spring    =    20;
            friction    =    0.5; // lower value = more friction (0 = no preservation of momentum)
            unstressed_spring_length = 100;
            delta = 0.4;
            movement_threshold = 0.5;
        }

        void TransformerSPRINGEMBEDDER::SpringEmbed(Graph& G, vector<float> dimension_limits, IntermediateStepHandler* intermediatestephandler)
        {
            vector<VectorND> tractions;
            int dimensions = dimension_limits.size();

            unstressed_spring_length = dimension_limits[0];
            for(int i = 1; i < dimensions; i++)
                unstressed_spring_length *= dimension_limits[i];
            unstressed_spring_length /= G.NumberOfVertices();
            unstressed_spring_length = pow(unstressed_spring_length, 1.0f/dimensions) / (dimensions * 2);

            // init
            srand ( time(NULL) );
            for(Graph::VertexIterator a = G.BeginVertices(); a != G.EndVertices(); a++)
            {
                vector<float> coordinates;
                for(int i = 0; i < dimensions; i++)
                    coordinates.push_back(fmod(rand(), dimension_limits[i]));
                a.SetCoordinates(coordinates);

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
                    if(sqrt(current_movement) > max_movement)
                        max_movement = sqrt(current_movement);
                }


                if(intermediatestephandler != NULL)
                    intermediatestephandler->Handle(&G);

                if(++iteration > nextincrease)
                {
                    movement_threshold++;
                    nextincrease *= 5;
                }
            }
            while( true);//(max_movement > movement_threshold*unstressed_spring_length));

        }

        void TransformerSPRINGEMBEDDER::DoTransform(Graph& G, vector<float> parameters, IntermediateStepHandler* intermediatestephandler)
        {
            int width = 800;
            int height = 600;

            if(parameters.size() > 1)
            {
                width = (int)(parameters[0]+0.5);
                height = (int)(parameters[1]+0.5);
            }

            vector<float> dimension_limits;
            dimension_limits.push_back(width);
            dimension_limits.push_back(height);
            SpringEmbed(G, dimension_limits, intermediatestephandler);
        }

    }
}
