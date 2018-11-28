#ifndef GRAPH_H
#define GRAPH_H
#include <list>
#include <vector>
#include <iostream>
#include <set>

enum Mark {
    PERMAMENT,
    TEMPORARY,
    UNMARKED
};

struct Vertex
{
    Vertex(int w, std::set<int>c) {weight = w; connections = c;};
    int weight;
    std::set <int> connections;
};

class Graph
{
    public:
        Graph();
        virtual ~Graph();
        int addVertex(int weight);
        void addEdge(int from, int to);
        void invertEdge(int from, int to);
        void removeEdge(int from, int to);
        void generateClique(const std::vector<int> &vertices);
        int getVertexWeigth(int vertex);

        std::vector<int> getDistances(int from);
        std::vector<int> getCriticalPath(int from, int to);
        std::list<int> topologicalSort();

        std::vector<Vertex> getVertices() {return adjacencyList;}
        void print();
    protected:

    private:
        int noVertices;

        void visit(int vertex);
        bool cyclic;
        std::list <int> topologicalOrder;
        std::vector <Mark> visited;

        std::vector <Vertex> adjacencyList;
};

#endif // GRAPH_H
