#include "../../opengt.so/Headers/opengt.h"
#include "../../opengt.so/Headers/algorithms/color/color.h"
#include <iostream>
using namespace std;
using namespace OpenGraphtheory;
using namespace OpenGraphtheory::Algorithms;

using namespace std;

int main(int argc, char** argv)
{
    // =================================================== TRANSLATE SUDOKU INTO A PRECOLORED GRAPH

    Graph G;
    Graph::VertexIterator board[9][9];
    map<Graph::VertexIterator, int> PreColoring;

    for(int y = 0; y < 9; y++)
        for(int x = 0; x < 9; x++)
        {
            board[y][x] = G.AddVertex(50*x + 50, 50*y + 50);

            int col;
            cin >> col;
            //if(col > 0)
                PreColoring[board[y][x]] = col-1;

            for(int xi = x-1; xi >= 0; xi--)
                G.AddEdge(board[y][x], board[y][xi]);
            for(int yi = y-1; yi >= 0; yi--)
                G.AddEdge(board[y][x], board[yi][x]);

            for(int yi = 3*(y/3); yi < y; yi++)
                for(int xi = 3*(x/3); xi < 3*(x/3)+3; xi++)
                    if(xi != x)
                        G.AddEdge(board[y][x], board[yi][xi]);
        }

    // ======================================== COMPUTE COLORING EXTENSION FOR THE PRECOLORED GRAPH

    AlgorithmCOLORING algo;
    if(algo.CompleteColoring(G, PreColoring, 9))
    {

    // ================================================= TRANSLATE COLORING BACK TO SUDOKU SOLUTION

        for(int y = 0; y < 9; y++)
        {
            for(int x = 0; x < 9; x++)
                cout << PreColoring[board[y][x]] + 1 << (x%3==2 ? "  " : " ");
            cout << (y%3==2 ? "\n\n" : "\n");
        }
    }
    else
    {
        cout << "no solution found\n";
    }

    return 0;
}
