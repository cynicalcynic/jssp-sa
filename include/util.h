#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

typedef std::vector<std::vector<std::pair<int, int> > > Data;

struct Instance
{
    int noJobs, noMachines;
    Data data;
};

#endif // UTIL_H_INCLUDED
