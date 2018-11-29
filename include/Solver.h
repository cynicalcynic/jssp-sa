#ifndef SOLVER_H
#define SOLVER_H
#include "Graph.h"
#include "util.h"

#include <random>
#include <vector>
#include <set>

struct Operation
{
    Operation(int time, int vertexId) : time(time), vertexId(vertexId) {};
    int time;
    int vertexId;
};


typedef std::pair<int, int> Edge;

class Solver
{
    public:
        Solver(const Instance &instance);
        virtual ~Solver();
        void simulatedAnnealing(const int kmax, const double startTemp, const double alpha, const int timeLimit);
        void printStartTimes();
    protected:

    private:
        void loadGraph(const Data &data);
        int calculateMakespan(const std::vector<int> &critPath);
        std::vector<Edge> findCriticalEdges();

        Graph g;
        int noJobs, noMachines;
        int source, sink;
        int bestMakespan;
        std::vector<int> bestDistances;
        std::vector<int> vertexWeigths; //contains weigths of each vertex
        std::vector<std::vector<Operation> > jobs; //jobs[i][j] contains vertex number of operation j of job i
        std::vector<std::vector<int> > machines; //machines[i] contains a vector of vertices numbers of operations executed on machine i

        std::mt19937 generator;
};

#endif // SOLVER_H
