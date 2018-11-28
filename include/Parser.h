#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <fstream>
#include <string>
#include <iostream>
#include "util.h"

#define BEASLEY 0
#define TAILARD 1

namespace Parser
{
    Instance load(std::string filename, int loadJobs = 0, int instanceType = 0)
    {
        if(instanceType == BEASLEY){
            std::ifstream file(filename);
            if(file.fail()) throw "file does not exist";
            int noJobs, noMachines;
            Instance instance;
            Data data;
            file>>noJobs>>noMachines;
            if(loadJobs > noJobs) throw "too many jobs specified";
            noJobs = loadJobs==0?noJobs:loadJobs;
            instance.noJobs = noJobs;
            instance.noMachines = noMachines;
            for(int i=0; i<noJobs; i++)
            {
                std::vector<std::pair<int, int> > job;
                for(int j=0; j<noMachines; j++)
                {
                    int machine, time;
                    file>>machine>>time;
                    job.push_back(std::make_pair(machine, time));
                }
                data.push_back(job);
            }
            instance.data = data;
            return instance;
        }
        else
        {
            std::ifstream file(filename);
            if(file.fail()) throw "file does not exist";
            std::string dummy;
            int noJobs, noMachines;
            Instance instance;
            file>>noJobs>>noMachines>>dummy>>dummy>>dummy>>dummy;
            if(loadJobs > noJobs) throw "too many jobs specified";
            std::vector<std::vector<int> > times;
            std::vector<std::vector<int> > machines;
            times.resize(noJobs);
            machines.resize(noJobs);
            Data data;
            file>>dummy;
            for(int i=0; i<noJobs; i++)
            {
                for(int j=0; j<noMachines; j++)
                {
                    int time;
                    file>>time;
                    times[i].push_back(time);
                }
            }
            file>>dummy;
            for(int i=0; i<noJobs; i++)
            {
                for(int j=0; j<noMachines; j++)
                {
                    int machine;
                    file>>machine;
                    machines[i].push_back(machine-1);
                }
            }


            noJobs = loadJobs==0?noJobs:loadJobs;
            instance.noJobs = noJobs;
            instance.noMachines = noMachines;
            for(int i=0; i<noJobs; i++)
            {
                std::vector<std::pair<int, int> > job;
                for(int j=0; j<noMachines; j++)
                {
                    job.push_back(std::make_pair(machines[i][j], times[i][j]));
                }
                data.push_back(job);
            }
            instance.data = data;
            return instance;
        }
    }

}

#endif // PARSER_H_INCLUDED
