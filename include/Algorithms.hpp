#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include "Graph.hpp"
#include <vector>

class Algorithms {
public:
    // Retorna a ordem topológica dos vértices. 
    // Se encontrar um ciclo, retorna um vetor vazio.
    static std::vector<int> topologicalSort(const Graph& graph);

    // Usa a ordem topológica para calcular o Caminho Máximo (Makespan).
    static int calculateMakespan(const Graph& graph, const std::vector<int>& topoOrder);

    // Retorna os IDs dos vértices com Grau de Entrada = 0
    static std::vector<int> getMinimalElements(const Graph& graph);

    // Retorna os IDs dos vértices com Grau de Saída = 0
    static std::vector<int> getMaximalElements(const Graph& graph);

    static std::vector<int> getCriticalPath(const Graph& graph, const std::vector<int>& topoOrder);
};

#endif