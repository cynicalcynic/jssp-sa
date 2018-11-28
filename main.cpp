#include <iostream>
#include <chrono>
#include "Solver.h"
#include "Parser.h"
#include "cxxopts.hpp"

using namespace std;

int main(int argc, char** argv)
{
    cxxopts::Options options("JSSP Simulated Annealing Scheduler");
    options.add_options()
    ("f,format", "Instance type", cxxopts::value<int>()->default_value("0"))
    ("j,jobs", "Number of jobs to load", cxxopts::value<int>()->default_value("0"))
    ("m,measure", "Measures execution time in nanoseconds")
    ("a,alpha", "Sets cooling factor", cxxopts::value<double>()->default_value("0.9999"))
    ("t, temp", "Sets starting temperature", cxxopts::value<double>()->default_value("150.0"))
    ("i, iterations", "Max iterations", cxxopts::value<int>()->default_value("10000"));
    if(argc == 1)
    {
        std::cout<<options.help()<<std::endl;
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
    solver.simulatedAnnealing(result["iterations"].as<int>(), result["temp"].as<double>(), result["alpha"].as<double>());
    solver.printStartTimes();
    return 0;
}
