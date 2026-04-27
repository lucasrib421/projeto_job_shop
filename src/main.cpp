#include <iostream>
#include "../include/JobShopModeler.hpp"
#include "../include/Graph.hpp"
#include "../include/Algorithms.hpp"
#include "../include/Heuristics.hpp"

int main() {
    JobShopModeler modeler;
    
    std::cout << "Lendo arquivo de instancia..." << std::endl;
    
    if (modeler.parseFile("../data/JSP/yn4Jsp.psi")) {
        std::cout << "Arquivo lido com sucesso!\n" << std::endl;
        
        // --- CENÁRIO 1: GRAFO RELAXADO ---
        Graph grafoInicial = modeler.buildInitialGraph();
        std::vector<int> ordemInicial = Algorithms::topologicalSort(grafoInicial);
        int makespanRelaxado = Algorithms::calculateMakespan(grafoInicial, ordemInicial);
        
        std::cout << "==========================================" << std::endl;
        std::cout << "MAKESPAN (Lower Bound / Ideal): \t" << makespanRelaxado << std::endl;
        std::cout << "==========================================\n" << std::endl;

        // --- CENÁRIO 2: SPT ---
        Graph grafoSPT = Heuristics::applySPT(grafoInicial, modeler.getTotalJobs(), modeler.getTotalMachines());
        std::vector<int> ordemSPT = Algorithms::topologicalSort(grafoSPT);
        int makespanSPT = Algorithms::calculateMakespan(grafoSPT, ordemSPT);
        
        std::cout << "==========================================" << std::endl;
        std::cout << "MAKESPAN (Heuristica SPT): \t\t" << makespanSPT << std::endl;
        std::cout << "==========================================\n" << std::endl;

        // --- CENÁRIO 3: LPT ---
        Graph grafoLPT = Heuristics::applyLPT(grafoInicial, modeler.getTotalJobs(), modeler.getTotalMachines());
        std::vector<int> ordemLPT = Algorithms::topologicalSort(grafoLPT);
        int makespanLPT = Algorithms::calculateMakespan(grafoLPT, ordemLPT);
        
        std::cout << "==========================================" << std::endl;
        std::cout << "MAKESPAN (Heuristica LPT): \t\t" << makespanLPT << std::endl;
        std::cout << "==========================================" << std::endl;

    } else {
        std::cout << "Falha ao ler o arquivo." << std::endl;
    }

    return 0;
}