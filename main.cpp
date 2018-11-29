#include <iostream>
#include <chrono>
#include "Solver.h"
#include "Parser.h"
#include "cxxopts.hpp"

using namespace std;

int main(int argc, char** argv)
{
    cxxopts::Options options("jobshop filename", "JSSP Simulated Annealing Scheduler");
    options.add_options()
    ("f, format", "Instance type (0 - beasley, 1 - taillard)", cxxopts::value<int>()->default_value("0"))
    ("j, jobs", "Number of jobs to load", cxxopts::value<int>()->default_value("0"))
    ("m, measure", "Measures execution time in nanoseconds")
    ("a, alpha", "Sets cooling factor", cxxopts::value<double>()->default_value("0.9999"))
    ("t, temp", "Sets starting temperature", cxxopts::value<double>()->default_value("150.0"))
    ("i, iterations", "Max iterations", cxxopts::value<int>()->default_value("100000"))
    ("l, limit", "Execution time limit", cxxopts::value<int>()->default_value("300"));
    if(argc == 1)
    {
        std::cout<<options.help()<<std::endl;
        return 0;
    }
    auto result = options.parse(argc, argv);
    Instance i;
    try
    {
        i = Parser::load(std::string(argv[1]), result["jobs"].as<int>(), result["format"].as<int>());
    }
    catch(const char* e)
    {
        std::cout<<e<<std::endl;
        return 1;
    }
    Solver solver(i);
    auto t1 = chrono::high_resolution_clock::now();
    solver.simulatedAnnealing(result["iterations"].as<int>(), result["temp"].as<double>(), result["alpha"].as<double>(), result["limit"].as<int>());
    auto t2 = chrono::high_resolution_clock::now();
    if(result.count("measure"))
        std::cout<<chrono::duration_cast<chrono::nanoseconds>( t2 - t1 ).count()<<std::endl;
    else
        solver.printStartTimes();
    return 0;
}
