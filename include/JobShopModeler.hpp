#ifndef JOBSHOPMODELER_HPP
#define JOBSHOPMODELER_HPP

#include <string>
#include <vector>
#include "Graph.hpp"

class JobShopModeler {
private:
    int totalJobs;
    int totalMachines;
    
    // Matriz para guardar os pesos lidos do bloco 'Costs:'
    // costs[job][machine] = tempo de processamento
    std::vector<std::vector<int>> costs;
    
    // Lista de pares (de -> para) para guardar as regras de cada Job
    // precedenceRules[job] = lista de arestas (máquinaA, máquinaB)
    std::vector<std::vector<std::pair<int, int>>> precedenceRules;

public:
    // Construtor
    JobShopModeler();

    // Função principal deste passo: Lê o TXT e preenche as variáveis acima
    bool parseFile(const std::string& filename);

    // Função auxiliar para calcular aquele ID único que discutimos
    int getOperationId(int job, int machine) const;

    // Getters
    int getTotalJobs() const;
    int getTotalMachines() const;
    int getCost(int job, int machine) const;
    const std::vector<std::pair<int, int>>& getRulesForJob(int job) const;

    //Constrói o grafo inicial com base nos dados lidos
    Graph buildInitialGraph() const;
};

#endif