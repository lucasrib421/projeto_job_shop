#include "../include/JobShopModeler.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

JobShopModeler::JobShopModeler() : totalJobs(0), totalMachines(0) {}

int JobShopModeler::getOperationId(int job, int machine) const {
    return (job * totalMachines) + machine;
}

bool JobShopModeler::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro: Não foi possível abrir o arquivo " << filename << std::endl;
        return false;
    }

    std::string line;
    std::string tempStr;

    // 1. LER CABEÇALHO (TotalJobs e TotalMachines)
    // Procuramos pelas palavras chave e lemos os números logo a seguir
    while (std::getline(file, line)) {
        if (line.find("TotalJobs:") != std::string::npos) {
            std::stringstream ss(line);
            ss >> tempStr >> totalJobs >> tempStr >> totalMachines;
            break;
        }
    }

    // Dimensionar as matrizes com os tamanhos lidos
    costs.assign(totalJobs, std::vector<int>(totalMachines, 0));
    precedenceRules.resize(totalJobs);

    // 2. LER MATRIZ DE CUSTOS (Costs:)
    while (std::getline(file, line)) {
        if (line.find("Costs:") != std::string::npos) {
            for (int i = 0; i < totalJobs; ++i) {
                std::getline(file, line);
                std::stringstream ss(line);
                for (int j = 0; j < totalMachines; ++j) {
                    ss >> costs[i][j];
                }
            }
            break; // Terminou de ler os custos
        }
    }

    // 3. LER REGRAS DE PRECEDÊNCIA (Job: X)
    int currentJob = 0;
    int jobIndex = 0;
    while (std::getline(file, line)) {
        if (line.find("Job:") != std::string::npos) {
            std::stringstream ss(line);
            ss >> tempStr >> currentJob; // Lê "Job:" e depois o número (que parece ser sempre o total-1 no seu txt)
               
            // Lê as linhas vazias ou com "->", até acabarem as regras desse job
            for (int i = 0; i < totalMachines - 1; ++i) {
                std::getline(file, line);
                // Pula linhas vazias
                while (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) {
                    std::getline(file, line);
                }
                
                int fromMachine, toMachine;
                std::string arrow;
                std::stringstream ruleStream(line);
                ruleStream >> fromMachine >> arrow >> toMachine;
                
                precedenceRules[jobIndex].push_back(std::make_pair(fromMachine, toMachine));
            }
            jobIndex++;
        }
    }

    file.close();
    return true;
}

// Implementação dos Getters
int JobShopModeler::getTotalJobs() const { return totalJobs; }
int JobShopModeler::getTotalMachines() const { return totalMachines; }
int JobShopModeler::getCost(int job, int machine) const { return costs[job][machine]; }
const std::vector<std::pair<int, int>>& JobShopModeler::getRulesForJob(int job) const { return precedenceRules[job]; }


Graph JobShopModeler::buildInitialGraph() const {
    // Calcula o total de vértices (Operações)
    int totalVertices = totalJobs * totalMachines;
    
    // Cria o grafo vazio com o tamanho correto
    Graph g(totalVertices);

    // ETAPA A: Adicionar todos os vértices e seus pesos
    for (int i = 0; i < totalJobs; ++i) {
        for (int j = 0; j < totalMachines; ++j) {
            int id = getOperationId(i, j); // Calcula o ID único
            int weight = getCost(i, j);    // Pega o tempo da matriz Costs
            
            g.addVertex(id, i, j, weight);
        }
    }

    // ETAPA B: Adicionar as arestas (Regras de Precedência de cada Job)
    for (int i = 0; i < totalJobs; ++i) {
        // Percorre cada regra "Máquina A -> Máquina B" que lemos do arquivo para o Job 'i'
        for (const auto& rule : precedenceRules[i]) {
            int fromMachine = rule.first;
            int toMachine = rule.second;

            // Descobre os IDs únicos dessas operações
            int fromId = getOperationId(i, fromMachine);
            int toId = getOperationId(i, toMachine);

            // Cria a aresta direcionada no grafo
            g.addEdge(fromId, toId);
        }
    }

    return g; // Retorna o grafo pronto!
}