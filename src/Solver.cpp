#include "Solver.h"
#include <vector>
#include <math.h>
#include <algorithm>
#include <chrono>

Solver::Solver(const Instance &instance)
{
    std::random_device rd;
    generator.seed(rd());
    noJobs = instance.noJobs;
    noMachines = instance.noMachines;
    loadGraph(instance.data);
}

Solver::~Solver()
{
    //dtor
}

int Solver::calculateMakespan(const std::vector<int> &critPath)
{
    int makespan = 0;
    for(int v : critPath)
    {
        makespan += vertexWeigths[v];
    }
    return makespan;
}

void Solver::loadGraph(const Data &instance)
{
    jobs.resize(noJobs);
    machines.resize(noMachines);
    vertexWeigths.resize(noJobs * noMachines + 2);

    //add source
    source = g.addVertex(0);
    for(size_t i = 0; i<instance.size(); ++i)
    {
        for(size_t j=0; j<instance[i].size(); ++j)
        {
            int num = g.addVertex(instance[i][j].second);
            jobs[i].push_back(Operation(instance[i][j].second, num));
            //add vertex number to machine vector
            machines[instance[i][j].first].push_back(num);
            //add vertex weigth
            vertexWeigths[num] = instance[i][j].second;
        }
    }
    //add last vertex
    sink = g.addVertex(0);

    //connect operations together
    for(size_t i = 0; i<instance.size(); ++i)
    {
        g.addEdge(source, jobs[i][0].vertexId);
        for(size_t j=1; j<instance[i].size(); ++j)
        {
            g.addEdge(jobs[i][j-1].vertexId, jobs[i][j].vertexId);
        }
        g.addEdge(jobs[i].back().vertexId, sink);
    }

    for(size_t i=0; i<machines.size(); ++i)
        g.generateClique(machines[i]);
}

std::vector<Edge> Solver::findCriticalEdges()
{
    auto critPath = g.getCriticalPath(source, sink);
    std::vector <std::pair<int, int> > feasibleEdges;
    for(size_t i = 0; i<critPath.size()-1; ++i)
    {
        for(auto v : machines)
        {
            if(std::find(v.begin(), v.end(), critPath[i]) != v.end() && std::find(v.begin(), v.end(), critPath[i+1]) != v.end())
            {
                feasibleEdges.push_back(std::make_pair(critPath[i], critPath[i+1]));
                break;
            }
        }
    }
    return feasibleEdges;
}

void Solver::printStartTimes()
{
    std::cout<<bestMakespan<<std::endl;
    for(size_t i=0; i<jobs.size(); ++i)
    {
        for(size_t j=0; j<jobs[i].size(); ++j)
        {
            std::cout<<bestDistances[jobs[i][j].vertexId] - jobs[i][j].time<<" ";;
        }
        std::cout<<std::endl;
    };
}

void Solver::simulatedAnnealing(const int kmax, const double startTemp, const double alpha, const int timeLimit)
{
    static std::uniform_real_distribution<>dis(0.0f, 1.0f);

    double temp = startTemp;
    bestDistances = g.getDistances(source);

    int makespan = calculateMakespan(g.getCriticalPath(source, sink));
    bestMakespan = makespan;

    auto beginTime = std::chrono::high_resolution_clock::now();
    bool timeElapsed = false;
    for(int k=0; k<kmax && !timeElapsed; ++k)
    {
        Edge edge;
        auto edges = findCriticalEdges();
        if(edges.empty())
            break;
        else
        {
            std::uniform_int_distribution<int> dis(0, edges.size()-1);
            edge = edges[dis(generator)];
        }
        g.invertEdge(edge.first, edge.second);

        int newMakespan = calculateMakespan(g.getCriticalPath(source, sink));

        if(newMakespan < makespan)
        {
            makespan = newMakespan;
            if(newMakespan < bestMakespan)
            {
                bestMakespan = newMakespan;
                bestDistances = g.getDistances(source);
            }
        }
        else
        {
            if(exp(double(makespan-newMakespan)/temp) > dis(generator))
            {
                makespan = newMakespan;
            }
            else
            {
                //go back to what was before
                g.invertEdge(edge.second, edge.first);
            }
        }

        int elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now() - beginTime).count();
        if(elapsed >= timeLimit)
            timeElapsed = true;

        temp *= alpha;
    }
}
