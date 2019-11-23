#include <iostream>
#include "graph.h"

int main()
{
    int verticesAmount, edgeBegin, edgeEnd;
    std::cin >> verticesAmount;
    Graph graph(verticesAmount);
    while(std::cin >> edgeBegin >> edgeEnd)
    {
        graph.addEdge(edgeBegin, edgeEnd);
    }
    graph.printBridges();
    return 0;
}