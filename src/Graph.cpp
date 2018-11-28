#include "Graph.h"
#include <algorithm>
#include <limits>

#define INF std::numeric_limits<int>::max()

Graph::Graph()
{
    noVertices = 0;
}

Graph::~Graph()
{

}

int Graph::addVertex(int weight)
{
    adjacencyList.push_back(Vertex(weight, std::set<int>()));
    noVertices++;
    return noVertices-1;
}

void Graph::addEdge(int from, int to)
{
    adjacencyList[from].connections.insert(to);
}

void Graph::print()
{
    for(size_t i=0; i<adjacencyList.size();i++)
    {
        std::cout<<"Vertex: "<<i<<" Weigth: "<<adjacencyList[i].weight<<" | ";
        for(auto it=adjacencyList[i].connections.begin(); it!=adjacencyList[i].connections.end(); it++)
        {
            std::cout<<*it<<" ";
        }
        std::cout<<std::endl;
    }
}

void Graph::removeEdge(int from, int to)
{
    adjacencyList[from].connections.erase(to);
}

void Graph::invertEdge(int from, int to)
{
    if(adjacencyList[from].connections.find(to) != adjacencyList[from].connections.end())
    {
        removeEdge(from, to);
        addEdge(to, from);
    }
    else
    {
        removeEdge(to, from);
        addEdge(from, to);
    }
}

void Graph::generateClique(const std::vector<int> &vertices)
{
    for(int i=0; i<vertices.size(); i++)
    {
        for(int j=i+1; j<vertices.size(); j++)
        {
            addEdge(vertices[i], vertices[j]);
        }
    }
}

int Graph::getVertexWeigth(int vertex)
{
    return adjacencyList[vertex].weight;
}

std::vector<int> Graph::getCriticalPath(int from, int to)
{
    topologicalSort();

    std::vector<int> distances(adjacencyList.size(), -INF);

    std::vector<int>prev(adjacencyList.size(), -1);
    distances[from] = 0;
    for(auto u=topologicalOrder.begin(); u!=topologicalOrder.end(); u++)
    {
        for(auto v=adjacencyList[*u].connections.begin(); v!=adjacencyList[*u].connections.end(); v++)
        {
            if(distances[*v] < distances[*u] + getVertexWeigth(*v))
            {
                distances[*v] = distances[*u] + getVertexWeigth(*v);
                prev[*v] = *u;
            }
        }
    }

    std::vector<int> path;
    int curr = to;
    path.push_back(curr);
    while(curr != 0)
    {
        path.push_back(prev[curr]);
        curr = prev[curr];
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<int> Graph::getDistances(int from)
{
    topologicalSort();

    std::vector<int> distances(adjacencyList.size(), -INF);
    distances[from] = 0;
    for(auto u=topologicalOrder.begin(); u!=topologicalOrder.end(); u++)
    {
        for(auto v=adjacencyList[*u].connections.begin(); v!=adjacencyList[*u].connections.end(); v++)
        {
            if(distances[*v] < distances[*u] + getVertexWeigth(*v))
            {
                distances[*v] = distances[*u] + getVertexWeigth(*v);
            }
        }
    }
    return distances;
}

std::list<int> Graph::topologicalSort()
{
    topologicalOrder.clear();
    visited.clear();
    visited.resize(adjacencyList.size(), UNMARKED);
    for(size_t i=0; i<adjacencyList.size(); i++)
    {
        visit(i);
    }
    return !cyclic?topologicalOrder:std::list<int>();
}

void Graph::visit(int vertex)
{
    if(visited[vertex] == PERMAMENT) return;
    if(visited[vertex] == TEMPORARY)
    {
        cyclic = true;
        return;
    } //won't happen
    visited[vertex] = TEMPORARY;
    for(auto it=adjacencyList[vertex].connections.begin(); it!=adjacencyList[vertex].connections.end(); it++)
    {
        visit(*it);
    }
    visited[vertex] = PERMAMENT;
    topologicalOrder.push_front(vertex);
}

