#ifndef HEURISTICS_HPP
#define HEURISTICS_HPP

#include "Graph.hpp"
#include <vector>

class Heuristics {
public:
    static Graph applySPT(const Graph& initialGraph, int totalJobs, int totalMachines);
    
    // NOVA HEURÍSTICA: LPT (Longest Processing Time)
    static Graph applyLPT(const Graph& initialGraph, int totalJobs, int totalMachines);

    static Graph applyMWKR(Graph grafoInicial, int jobs, int machines);
};

#endif