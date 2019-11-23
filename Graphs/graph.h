#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <iostream>

class Graph
{
    std::vector<std::vector<std::size_t>> edges;

    void DFS(int v, bool *visited)
    {
        visited[v] = true;
        for(std::size_t i = 0; i != edges[v].size(); ++i)
        {
            if(visited[edges[v][i]] == false)
                DFS(edges[v][i], visited);
        }
    }
    
    int countComponents(int v, int w)
    {
        int componentsAmount = 0;
        bool *visited = new bool[edges.size()];
        visited[v] = true;
        visited[w] = true;
        for(std::size_t i = 0; i != edges.size(); ++i)
        {
            if(visited[i] == false)
            {
                DFS(i, visited);
                ++componentsAmount;
            }
        }
        delete[] visited;
        return componentsAmount;
    }

public:
    Graph(int verticesAmount_) : edges(verticesAmount_)
    {}

    void addEdge(int v, int w)
    {
        edges[v].push_back(w);
        edges[w].push_back(v);
    }

    void printBridges()
    {
        for(std::size_t i = 0; i != edges.size(); ++i)
        {
            for(std::size_t j = 0; j != edges[i].size(); ++j)
            {
                if(i < edges[i][j] && countComponents(i, edges[i][j]) > 1)
                {
                    std::cout << i << " " << edges[i][j] << std::endl;
                }
            }
        }
    }
};

#endif